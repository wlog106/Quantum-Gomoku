#include "share_cmd.h"
#include "share_wrap.h"
#include <server_utils.h>
#include <server_cmd.h>
#include <cstring>
#include <server_objects.h>
#include <sys/socket.h>

void on_observer_join(Game *g, int fd){
    struct msghdr msg = {0};
    struct cmsghdr* cmsg;
    char iovbuf[65];
    int ffd[1];
    struct iovec iov = {
        iovbuf, 
        sizeof(iovbuf)
    };
    union {
        char buf[CMSG_SPACE(sizeof(ffd))];
        struct cmsghdr align;
    } u;

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = u.buf;
    msg.msg_controllen = sizeof(u.buf);
    

    int n = recvmsg(fd, &msg, 0);
    cmsg = CMSG_FIRSTHDR(&msg);
    conn *newConn = g->get_empty_observe_pos();
    if(newConn == NULL){
        printf("error: no empty space of new observer\n");
        return;
    }
    if (cmsg && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS){
        memcpy(ffd, CMSG_DATA(cmsg), sizeof(ffd));
        memcpy(newConn->name, msg.msg_iov->iov_base, sizeof(msg.msg_iov->iov_len));
        newConn->fd = ffd[0];
        printf("New observer name: %s fd: %d join room: %s\n",
               newConn->name, newConn->fd, g->room_id);
    }
    else{
        printf("error on receiving new observer fd\n");
    }
    job_t *newJob = new job_t;
    newJob->type = RES_USR;
    char *cmd = (char*)malloc(MAXLINE*sizeof(char));
    sprintf(cmd, "%d %s %s %d\n%d 0 %lld %lld %d\n",
            C_start_a_playing_room, g->room_id,
            g->get_full_game_info().data(), g->get_pos(newConn),
            C_playing_new_segement, g->p1_time, g->p2_time, g->cur_player+1);
    newJob->fill_line(cmd);
    push_res_job(newConn->jobq, newJob);
    epoll_rw_add(g->epfd, newConn->fd);
}