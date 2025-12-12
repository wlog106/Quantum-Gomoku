#include <share_wrap.h>
#include <server_objects.h>

conn::conn(){
    this->r_buf = new linear_buf_t(MAXLINE);
}

conn::conn(int fd, int state){ 
    this->fd = fd;
    this->state = state;
    this->r_buf = new linear_buf_t(MAXLINE);
}

conn::~conn(){
    delete r_buf;
}