#include <server_objects.h>

ServerContext::ServerContext(){}

ServerContext::ServerContext(
    int epfd, 
    int listenfd,
    int dw_fd,
    int rmgr_fd
){
    this->epfd = epfd;
    this->listenfd = listenfd;
    this->dw_fd = dw_fd;
    this->rmgr_fd = rmgr_fd;
}