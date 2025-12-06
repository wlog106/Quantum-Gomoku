#include "../server_.h"

#include <cassert>
#include <cerrno>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

void on_writable(
    ServerContext *scxt,
    ServerObjects *sobj
){
    int n;
    job_t *cur_job;
    if(scxt->cur_fd == scxt->dw_fd){
        while(!sobj->dwq->empty()){
            cur_job = sobj->dwq->front();
            n = write(
                scxt->dw_fd, 
                cur_job->r_ptr, 
                cur_job->len
            );
            if(n == -1){
                if(errno == EINTR)
                    continue;
                else if(errno == EAGAIN || errno == EWOULDBLOCK)
                    goto wouldblock_try_later;
            }
            /* no error */
            if(n < cur_job->len){
                cur_job->r_ptr += n;
                cur_job->len -= n;
            }
            else if(n == cur_job->len){
                free(cur_job->line);
                free(cur_job);
                sobj->dwq->pop();
            }
        }
        if(sobj->dwq->empty()) 
            goto del_cur_fd_from_epollout;
    }
    else if(scxt->cur_fd == scxt->rmgr_fd){

    }
    else{
        conn *u;
        u = get_user(scxt->cur_fd, sobj->fd_to_conn);
        cur_job = u->jobq.front();
        if(u->db_job_pending || cur_job->type != RES_USR)
            goto del_cur_fd_from_epollout;
        while(1){
            n = write(u->fd, cur_job->r_ptr, cur_job->len);
            if(n == -1){
                if(errno == EINTR)
                    continue;
                if(errno == EWOULDBLOCK || errno == EAGAIN)
                    goto wouldblock_try_later;
            }
            /* no error */
            if(n < cur_job->len){
                cur_job->r_ptr += n;
                cur_job->len -= n;
            }
            else if(n == cur_job->len){
                free(cur_job->line);
                free(cur_job);
                u->jobq.pop();
                /* set the needs for next job */
                assert(1==0);
            }
        }
    }

del_cur_fd_from_epollout:
    struct epoll_event ev;
    ev.events = EPOLLET;
    ev.data.fd = scxt->cur_fd;
    Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, scxt->cur_fd, &ev);

wouldblock_try_later:
    return;
}