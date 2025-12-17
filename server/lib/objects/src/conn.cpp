#include <sys/timerfd.h>
#include <share_wrap.h>
#include <server_objects.h>
#include <server_utils.h>

conn::conn(){
    this->room_id = "ROOMLESS";
    this->r_buf = new linear_buf_t(MAXLINE);
    start = {
        {0, 0},
        {2, 0}
    };
    stop = {0};
    tfd = timerfd_create(
        CLOCK_MONOTONIC, 
        TFD_NONBLOCK
    );
}

conn::conn(int fd, int state){ 
    this->fd = fd;
    this->state = state;
    this->room_id = "ROOMLESS";
    this->r_buf = new linear_buf_t(MAXLINE);
    start = {
        {0, 0},
        {2, 0}
    };
    stop = {0};
    tfd = timerfd_create(
        CLOCK_MONOTONIC, 
        TFD_NONBLOCK
    );
}

conn::~conn(){
    Close(tfd);
    delete r_buf;
}

void conn::start_timer(){
    timerfd_settime(tfd, 0, &start, NULL);
}

void conn::stop_timer(){
    timerfd_settime(tfd, 0, &stop, NULL);
}