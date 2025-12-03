#include "../../server_.h"
#include <bits/types/struct_iovec.h>
#include <sys/epoll.h>
#include <sys/uio.h>

void on_report(DwContext *dwcxt){
    struct iovec iov[1];
    int iocnt;
    Dw_response_t *res = new Dw_response_t(dwcxt->resultq->front());
    iov[0].iov_base = res;
    iov[0].iov_len = sizeof(Dw_response_t);
    iocnt = sizeof(iov) / sizeof(iovec);
    writev(dwcxt->mainfd, iov, iocnt);
    dwcxt->resultq->pop();
    if(dwcxt->resultq->empty()){
        struct epoll_event ev;
        ev.events = EPOLLET;
        ev.data.fd = dwcxt->mainfd;
        Epoll_ctl(dwcxt->epfd, EPOLL_CTL_MOD, dwcxt->mainfd, &ev);
    }
}