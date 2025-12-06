#include "../../server_.h"
#include <bits/types/struct_iovec.h>
#include <cerrno>
#include <sys/epoll.h>
#include <sys/uio.h>

void on_response(DwContext *dwcxt){
    int n;
    job_res *cur_res;
    while(!dwcxt->resultq->empty()){
        cur_res = &dwcxt->resultq->front();
        n = write(
            dwcxt->mainfd,
            cur_res->r_ptr,
            cur_res->len
        );
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        /* no error */
        if(n < cur_res->len){
            cur_res->r_ptr += n;
            cur_res->len -= n;
        }
        else if(n == cur_res->len){
            free(cur_res->line);
            dwcxt->resultq->pop();
        }
    }
    if(dwcxt->resultq->empty()){
        struct epoll_event ev;
        ev.events = EPOLLET;
        ev.data.fd = dwcxt->mainfd;
        Epoll_ctl(dwcxt->epfd, EPOLL_CTL_MOD, dwcxt->mainfd, &ev);
    }
}