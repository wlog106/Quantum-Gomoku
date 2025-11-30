#include "io.h"
#include <cerrno>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

void on_listen(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
){
    int newfd, flags;
    conn_t *newConn;
    struct epoll_event ev;
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    while(1){
        newfd = accept(scxt->listenfd,  (SA *)&cliaddr, &clilen);
        if(newfd == -1){
            if(errno == EINTR || errno == ECONNABORTED || errno == EPROTO)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
            else
                /* error */;
        }
        flags = Fcntl(newfd, F_GETFL, 0);
        Fcntl(newfd, F_SETFL, flags | O_NONBLOCK);
        ev.events = EPOLLET;
        ev.data.fd = newfd;
        Epoll_ctl(scxt->epfd, EPOLL_CTL_ADD, newfd, &ev);
        newConn = new conn_t(newfd);
        fd_to_conn.insert({newfd, newConn});
    }
}