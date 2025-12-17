#include <server_utils.h>
#include <bits/types/struct_iovec.h>
#include <fcntl.h>
#include <server_objects.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../dispatcher/dispatcher.h"

void on_room_msg(
    ServerContext *scxt,
    ServerObjects *sobj
){
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char iovbuf[300];
    int fds[1];
    struct iovec iov = {
        iovbuf,
        sizeof(iovbuf)
    };
    union {
        char buf[CMSG_SPACE(sizeof(fds))];
        struct cmsghdr align;
    } u;

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = u.buf;
    msg.msg_controllen = sizeof(u.buf);

    int n = TEMP_FAILURE_RETRY(recvmsg(scxt->cur_fd, &msg, 0));
    if(n == 0){
        printf("a room die...\n");
        epoll_del(scxt->epfd, scxt->cur_fd);
        Close(scxt->cur_fd);
        return;
    }
    cmsg = CMSG_FIRSTHDR(&msg);
    char room_msg[300];
    if (cmsg && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS){
        memcpy(fds, CMSG_DATA(cmsg), sizeof(fds));
        memcpy(room_msg, msg.msg_iov->iov_base, msg.msg_iov->iov_len);
        printf("receive room msg %s from room successfully\n", room_msg);
        printf("transfer: %d bytes\n", n);
        printf("examine fd: %d\n", fcntl(fds[0], F_GETFD));
    }
    else{
        printf("error on receiving room msg's fds\n");
    }
    room_msg_dispatcher(scxt, sobj, room_msg, fds[0]);
}