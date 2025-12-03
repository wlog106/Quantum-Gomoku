#include "../../server_.h"

DwContext::DwContext(){}


DwContext::DwContext(
    int mainfd, 
    int epfd,
    db_conn *dc, 
    std::queue<Dw_response_t> *rq
){
    this->mainfd = mainfd;
    this->epfd = epfd;
    this->dc = dc;
    this->resultq = rq;
}