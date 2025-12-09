#include <share_wrap.h>
#include <server_objects.h>
#include <server_utils.h>
#include <mariadb.h>

#include "dw_io/dw_io.h"

#include <sys/epoll.h>
#include <csignal>
#include <queue>

int main(int arg, char** argv){
    //raise(SIGSTOP);
    int mainfd;
    db_conn *db_handler;
    int nfds;
    int epfd;
    struct epoll_event ev, events[MAX_EVENT];
    std::queue<job_res> resultq;
    std::queue<job_t> jobq; 
    linear_buf_t stream_buf(MAXLINE);

    sscanf(argv[1], "%d", &mainfd);

    db_handler = db_init();

    epfd = Epoll_create();

    ev.events = EPOLLIN | EPOLLET;
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
        std::cout << "(dw) epoll in db worker wait for jobs\n";
        nfds = Epoll_wait(epfd, events, MAX_EVENT);
        std::cout << "(dw) get job\n";
        for(int i=0; i<nfds; i++){
            if(events[i].events & EPOLLIN){
                on_recv_job(dwcxt);
            }
            else if(events[i].events & EPOLLOUT){
                on_response(dwcxt);
            }
        }
    }
    std::cout << "db_worker die\n";
}