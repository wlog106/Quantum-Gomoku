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
    ev.events = EPOLLIN | EPOLLET;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_r_mod(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    Epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void epoll_rw_add(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_rw_mod(
    int epfd,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
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
        if((*it)->type != RES_USR) break;
        it++;
    }
    jobq.insert(it, newJob);
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

    Socketpair(AF_UNIX, SOCK_STREAM, 0, room_fd);
    // remove conn from fd_to_conn
    for(int i=0; i<5; i++){
        if(!room->user_existance[i])
            continue;
        sobj->fd_to_conn->erase(room->users[i]->fd);
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
    // close unecessary fds
    room->close_exist_userfds(epfd);
    Close(room_fd[1]);
    room->is_playing = true;
    return child_pid;
}