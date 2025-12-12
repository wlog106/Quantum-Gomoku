#include <fcntl.h>
#include <share_wrap.h>
#include <server_objects.h>
#include <server_utils.h>
#include <server_cmd.h>

#include <cerrno>
#include <sys/epoll.h>

void on_listen(
    ServerContext *scxt,
    ServerObjects *sobj
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
        /* set non_block and close when execute */
        Fcntl(newfd, F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC);
        epoll_r_add(scxt->epfd, newfd);
        newConn = new conn(newfd, UNKNOWN_USR);
        sobj->fd_to_conn->insert({newfd, newConn});
        std::cout << "(serv_main) new connection launched, peer fd: " << newfd << "\n";
    }
}