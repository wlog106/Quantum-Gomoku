#include "../server_.h"
#include <sys/epoll.h>

void dw_res_processor(
    ServerContext *scxt,
    std::queue<dw_res> &q
){
    while(!q.empty()){
        dw_res *cur_dw_res = &q.front();
        job_t *newJob = new job_t;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        struct epoll_event ev;
        switch (cur_dw_res->type)
        {
            case (DW_SIGNUP):
                if(cur_dw_res->result == DW_RESULT_SUCCESS){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_login_success);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    cur_dw_res->u->jobq.push(newJob);
                    cur_dw_res->u->state = LOGGEDIN_USR;
                }
                else if(cur_dw_res->result == DW_ERESULT_DUPNAME){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_account_already_exist);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    cur_dw_res->u->jobq.push(newJob);
                    //cur_dw_res->u->state = UNKNOWN_USR;
                }
                break;
            case (DW_LOGIN):
                if(cur_dw_res->result == DW_RESULT_SUCCESS){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_login_success);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    cur_dw_res->u->jobq.push(newJob);
                    cur_dw_res->u->state = LOGGEDIN_USR;
                }
                else if(cur_dw_res->result == DW_ERESULT_USERDNE){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_account_does_not_exist);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    cur_dw_res->u->jobq.push(newJob);
                    //cur_dw_res->u->state = UNKNOWN_USR;
                }
                else if(cur_dw_res->result == DW_ERESULT_PWDFAIL){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_password_incorrect);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    cur_dw_res->u->jobq.push(newJob);
                    //cur_dw_res->u->state = UNKNOWN_USR;
                }
                break;
        }
        if(cur_dw_res->u->jobq.size() == 1){
            struct epoll_event ev;
            ev.events = EPOLLET | EPOLLOUT;
            ev.data.fd = cur_dw_res->u->fd;
            Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, cur_dw_res->u->fd, &ev);
        }
    }
}