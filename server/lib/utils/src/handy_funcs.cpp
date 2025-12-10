#include <server_objects.h>
#include <server_utils.h>

#include <map>
#include <sys/epoll.h>

conn *get_user(
    int fd, 
    std::map<int, conn*> *fd_to_conn
){
    auto it = fd_to_conn->find(fd);
    return it->second;
}

void epoll_rw_mod(
    ServerContext *scxt,
    int fd
){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, fd, &ev);
}