#include <ctime>
#include <share_wrap.h>
#include <server_objects.h>

conn::conn(){
    this->observed_sent = 0;
    this->r_buf = new linear_buf_t(MAXLINE);
}

conn::conn(int fd, int state){ 
    this->fd = fd;
    this->state = state;
    this->observed_sent = 0;
    this->r_buf = new linear_buf_t(MAXLINE);
}

conn::~conn(){
    delete r_buf;
}

long long conn::get_time(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000) - observed_sent;
}

void conn::set_time(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    observed_sent = (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}