#include <universal.h>
#include <server_objects.h>
#include <server_utils.h>
#include <server_cmd.h>

#include <cerrno>
#include <sys/epoll.h>

void on_listen(
    ServerContext *scxt,
    std::map<int, conn*> &fd_to_conn
){
    int newfd, flags;
    conn *newConn;
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
            else{
                /* error */;
            }
        }
        flags = Fcntl(newfd, F_GETFL, 0);
        Fcntl(newfd, F_SETFL, flags | O_NONBLOCK);
        ev.events = EPOLLET;
        ev.data.fd = newfd;
        Epoll_ctl(scxt->epfd, EPOLL_CTL_ADD, newfd, &ev);
        newConn = new conn(newfd, UNKNOWN_USR);
        fd_to_conn.insert({newfd, newConn});
    }
}