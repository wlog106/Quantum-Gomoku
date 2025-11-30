#include "../server_.h"


conn_t::conn_t(){assert(1==0);}

conn_t::conn_t(int fd){
    id = -1;
    this->fd = fd;
    r_buf = linear_buf_t(MAXLINE);
    w_buf = linear_buf_t(MAXLINE);
}