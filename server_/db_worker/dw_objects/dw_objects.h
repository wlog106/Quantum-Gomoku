#ifndef DW_OBJECTS_H
#define DW_OBJECTS_H

#include <deque>
#include <queue>

struct linear_buf_t;
struct job_t;
struct db_conn;

struct job_res {
    char *line;
    char *r_ptr;
    int len;
};

struct DwContext{
    int mainfd, epfd;
    db_conn *dc;
    std::queue<job_res> *resultq;
    linear_buf_t *stream_buf;
    bool have_pending_res;
    std::queue<job_t> *jobq;
    DwContext();
    DwContext(
        int mainfd, 
        int epfd,
        db_conn *dc, 
        std::queue<job_res> *resultq,
        linear_buf_t *stream_buf,
        std::queue<job_t> *jobq
    );
};

#endif