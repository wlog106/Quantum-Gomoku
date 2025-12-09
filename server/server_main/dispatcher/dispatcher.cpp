#include "share_cmd.h"
#include "share_wrap.h"
#include <server_cmd.h>
#include <server_objects.h>
#include <server_utils.h>
#include <sys/epoll.h>

#define MAX_ROOM 20

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
        else if(cur_job->type == CREATE_ROOM){
            job_t *newJob = new job_t;
            char *cmd = (char*)malloc(MAXLINE*sizeof(char));
            if(sobj->id_to_room->size() == MAX_ROOM){
                newJob->type = RES_USR;
                sprintf(cmd, "%d\n", C_too_much_room);
                newJob->line = cmd;
                newJob->r_ptr = cmd;
                newJob->len = strlen(cmd);
                u->jobq.push_front(newJob);
            }
            else{
                
            }
        }
    }
}