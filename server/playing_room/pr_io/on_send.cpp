#include <server_utils.h>
#include <server_cmd.h>
#include <server_objects.h>
#include <unistd.h>

#include "../pr_dispatcher/pr_dispatcher.h"

void on_send(
    Game *g,
    int fd
){
    int n;
    conn *u;
    job_t *cur_job;
    u = g->get_user(fd);
    while(!u->jobq.empty()){
        cur_job = u->jobq.front();
        if(cur_job->type != RES_USR 
        && cur_job->type != SEND_OBSERVE_RESULT){
            break;
        }
        /* time gap between show observe result and new seg/game over */
        if(cur_job->type == RES_USR && u->get_time() < 50)
            return;

        n = write(u->fd, cur_job->r_ptr, cur_job->len);
        if(n == -1){
            if(errno == EINTR)
                continue;
            if(errno == EWOULDBLOCK || errno == EAGAIN)
                return;
        }
        if(n < cur_job->len){
            cur_job->r_ptr += n;
            cur_job->len -= n;
        }
        else if(n == cur_job->len){
            if(cur_job->type == SEND_OBSERVE_RESULT)
                u->set_time();
            free(cur_job->line);
            delete cur_job;
            u->jobq.pop_front();
            pr_dispatcher(g, u);
        }
    }
    epoll_r_mod(g->epfd, u->fd);
}