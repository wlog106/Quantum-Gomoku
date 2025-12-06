#include "../../server_.h"
#include "dw_objects.h"

DwContext::DwContext(){}


DwContext::DwContext(
    int mainfd, 
    int epfd,
    db_conn *dc, 
    std::queue<job_res> *rq,
    linear_buf_t *stream_buf,
    std::queue<job_t> *jobq
){
    this->mainfd = mainfd;
    this->epfd = epfd;
    this->dc = dc;
    this->resultq = rq;
    this->stream_buf = stream_buf;
    this->jobq = jobq;
    this->have_pending_res = false;
}