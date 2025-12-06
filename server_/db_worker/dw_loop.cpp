#include "../server_.h"
#include "dw_objects/dw_objects.h"
#include <bits/types/struct_iovec.h>
#include <cerrno>
#include <cstdio>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <queue>

int main(int arg, char** argv){
    int mainfd;
    db_conn *db_handler;
    int nfds;
    int epfd;
    struct epoll_event ev, events[MAX_EVENT];
    std::queue<job_res> resultq;
    std::queue<job_t> jobq; 
    linear_buf_t stream_buf(MAXLINE);

    sscanf(argv[0], "%d", &mainfd);

    db_handler = db_init();

    epfd = Epoll_create();

    ev.events = EPOLLET;
    ev.data.fd = mainfd;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, mainfd, &ev);

    DwContext *dwcxt = new DwContext(
        mainfd, 
        epfd, 
        db_handler, 
        &resultq,
        &stream_buf,
        &jobq
    );

    for( ; ; ){
        nfds = Epoll_wait(epfd, events, MAX_EVENT);
        for(int i=0; i<nfds; i++){
            if(events[i].events & EPOLLET){
                on_recv_job(dwcxt);
            }
            else if(events[i].events & EPOLLOUT){
                on_report(dwcxt);
            }
        }
        
    }

}