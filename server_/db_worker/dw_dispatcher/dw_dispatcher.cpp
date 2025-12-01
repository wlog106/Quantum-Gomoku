#include "../../server_.h"
#include <sys/epoll.h>

void dw_dispatcher(
    int n, 
    struct iovec *iov,
    DwContext *dwcxt
){
    job_t *job_ptr;
    struct epoll_event ev;
    for(int i=0; i<n; i++){
        job_ptr = (job_t *)iov[i].iov_base;
        switch(job_ptr->action)
        {
            case(DB_SIGNUP):
                dwcxt->resultq->push(dw_signup());
                break;
            case(DB_LOGIN):
                dwcxt->resultq->push(dw_login());
                break;
        }
    }
    ev.events = EPOLLOUT;
    ev.data.fd = dwcxt->mainfd;
    Epoll_ctl(dwcxt->epfd, EPOLL_CTL_MOD, dwcxt->mainfd, &ev);
}