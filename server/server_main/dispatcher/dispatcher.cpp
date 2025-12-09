#include <server_cmd.h>
#include <server_objects.h>
#include <server_utils.h>
#include <sys/epoll.h>

void dispatcher(
    ServerContext *scxt,
    ServerObjects *sobj,
    conn *u
){
    job_t *cur_job = u->jobq.front();
    while(!u->jobq.empty()){
        if(cur_job->type == DW_SIGNUP || cur_job->type == DW_LOGIN){
            /* db_worker */
            if(sobj->dwq->empty()){
                struct epoll_event ev;
                ev.data.fd = scxt->dw_fd;
                ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
                Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, scxt->dw_fd, &ev);
            }
            sobj->dwq->push(cur_job);
            u->jobq.pop_front();
            break;
        }
        else if(cur_job->type == RES_USR){
            struct epoll_event ev;
            ev.data.fd = u->fd;
            ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
            Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, u->fd, &ev);
            break;
        }
        else{

        }
    }
}