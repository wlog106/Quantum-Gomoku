#include "server_cmd.h"
#include <cstdio>
#include <deque>
#include <fcntl.h>
#include <server_objects.h>
#include <server_utils.h>

#include <map>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>

conn *get_user(
    int fd, 
    std::map<int, conn*> *fd_to_conn
){
    auto it = fd_to_conn->find(fd);
    return it->second;
}

void epoll_r_add(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_r_mod(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    Epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void epoll_rw_add(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_rw_mod(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT;
    Epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void epoll_del(
    int epfd,
    int fd
){
    Epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

void push_res_job(
    std::deque<job_t*> &jobq,
    job_t *newJob
){
    auto it = jobq.begin();
    while(it!=jobq.end()){
        if((*it)->type != RES_USR 
        && (*it)->type != SEND_OBSERVE_RESULT)
            break;
        it++;
    }
    jobq.insert(it, newJob);
}

void pass_ufd_to_room(
    ServerContext *scxt,
    ServerObjects *sobj,
    Room *room, 
    conn *u
){
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    int fd_to_pass[1] = {u->fd};
    char iobuf[65];
    strncpy(iobuf, u->name, 65);
    struct iovec iov = {
        iobuf,
        sizeof(iobuf)
    };
    
    union {         
        char buf[CMSG_SPACE(sizeof(fd_to_pass))];
        struct cmsghdr align;
    } uunn;

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = uunn.buf;
    msg.msg_controllen = sizeof(uunn.buf);
    
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_pass));
    memcpy(CMSG_DATA(cmsg), fd_to_pass, sizeof(fd_to_pass));

    /* only check EINTR signal */
    int n = TEMP_FAILURE_RETRY(sendmsg(room->room_fd, &msg, 0));
    if(n == -1) {
        printf("error on passing observer fd: %d, errno: %d\nno change has been made...\n", 
                *fd_to_pass, errno);
        return;
    }
    else{
        printf("send observer: %s's fd to room: %s successfully!\n",
               u->name, room->room_id.data());
    }

    /* remove user on parent side */
    sobj->fd_to_conn->erase(u->fd);
    epoll_del(scxt->epfd, u->fd);
    Close(u->fd);
    u->fd = -1;
}

pid_t fork_room(
    ServerObjects *sobj,
    Room *room,
    int epfd
){
    pid_t child_pid;
    int room_fd[2];
    unsigned int exist_pos = 0;
    char room_proc_name[30];
    char room_metadata[34];
    char exist_usernames[150];
    char exist_userfds[15];

    /* 
        use SOCK_DGRAM to avoid partial read/write
    */
    Socketpair(AF_UNIX, SOCK_DGRAM, 0, room_fd);
    // remove conn from fd_to_conn
    for(int i=0; i<5; i++){
        if(!room->user_existance[i])
            continue;
        exist_pos |= (1 << i);
    }
    if((child_pid = Fork()) == 0){
        // setup IPC tunnel
        Close(room_fd[0]);
        int flags = Fcntl(room_fd[1], F_GETFL, 0);
        Fcntl(room_fd[1], F_SETFL, flags | O_NONBLOCK );
        // prepare necessary data
        sprintf(room_proc_name, "playing room: %s",
                room->room_id.data());
        sprintf(room_metadata, "%d %s %u", 
                room_fd[1], room->room_id.data(), exist_pos);
        sprintf(exist_usernames, "%s", 
                room->get_exist_usernames().data());
        sprintf(exist_userfds, "%s",
                room->get_exist_userfds().data());
        char *room_argv[] = {
            room_proc_name,
            room_metadata,
            exist_usernames,
            exist_userfds,
            NULL
        };
        // turn off exist users' FD_CLOEXEC option
        room->turn_off_fd_close_on_exec();
        Execv("./playing_room", room_argv);
        assert(false);
    }
    // remove unecessary fds on parent side
    room->erase_exist_userinfo(sobj, epfd);
    Close(room_fd[1]);
    int flags = Fcntl(room_fd[0], F_GETFL, 0);
    Fcntl(room_fd[0], F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC);
    room->is_playing = true;
    room->room_fd = *room_fd;
    sobj->playing_room_fds->insert(*room_fd);
    epoll_r_add(epfd, *room_fd);
    return child_pid;
}

bool is_playing_room(
    ServerObjects *scxt,
    int fd
){
    return (
        scxt->playing_room_fds->size() != 0 &&
        scxt->playing_room_fds->find(fd)
        != scxt->playing_room_fds->end()
    );
}