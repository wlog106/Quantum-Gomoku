#ifndef DW_OBJECTS_H
#define DW_OBJECTS_H

#include <queue>

struct conn_t;
struct db_conn;

struct Dw_response_t{
    int dw_job;
    int dw_result;
    conn_t *user;
    Dw_response_t();
    Dw_response_t(int dw_job);
};

struct DwContext{
    int mainfd, epfd;
    db_conn *dc;
    std::queue<Dw_response_t> *resultq;
    DwContext();
    DwContext(
        int mainfd, 
        int epfd,
        db_conn *dc, 
        std::queue<Dw_response_t> *rq
    );
};

#endif