#ifndef DB_WORKER_H
#define DB_WORKER_H

#include "dw_io/dw_io.h"
#include "dw_dispatcher/dw_dispatcher.h"

#include <queue>

struct conn_t;
struct db_conn;

struct DB_result_t{
    int type;
    int result;
    conn_t *user;
};

struct DwContext{
    int mainfd, epfd;
    db_conn *dc;
    std::queue<DB_result_t> *resultq;
    DwContext(){}
    DwContext(
        int mainfd, 
        int epfd,
        db_conn *dc, 
        std::queue<DB_result_t> *rq
    ){
        this->mainfd = mainfd;
        this->epfd = epfd;
        this->dc = dc;
        this->resultq = rq;
    }
};

#endif