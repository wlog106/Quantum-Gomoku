#include <share_wrap.h>
#include <share_cmd.h>
#include <server_cmd.h>
#include <server_objects.h>
#include <server_utils.h>

#include "../dispatcher/dispatcher.h"

#include <sys/epoll.h>

void dw_res_processor(
    ServerContext *scxt,
    ServerObjects *sobj,
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
                    push_res_job(cur_dw_res->u->jobq, newJob);
                    cur_dw_res->u->state = OP_SELECTING_USR;
                }
                else if(cur_dw_res->result == DW_ERESULT_DUPNAME){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_account_already_exist);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    push_res_job(cur_dw_res->u->jobq, newJob);
                    //cur_dw_res->u->state = UNKNOWN_USR;
                }
                break;
                
            case (DW_LOGIN):
                if(sobj->login_ids->find(cur_dw_res->u->id)!=sobj->login_ids->end()){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_already_login);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    push_res_job(cur_dw_res->u->jobq, newJob);
                }
                else if(cur_dw_res->result == DW_RESULT_SUCCESS){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_login_success);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    push_res_job(cur_dw_res->u->jobq, newJob);
                    cur_dw_res->u->state = OP_SELECTING_USR;
                    sobj->login_ids->insert(cur_dw_res->u->id);
                }
                else if(cur_dw_res->result == DW_ERESULT_USERDNE){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_account_does_not_exist);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    push_res_job(cur_dw_res->u->jobq, newJob);
                    //cur_dw_res->u->state = UNKNOWN_USR;
                }
                else if(cur_dw_res->result == DW_ERESULT_PWDFAIL){
                    newJob->type = RES_USR;
                    sprintf(cmd, "%d\n", C_password_incorrect);
                    newJob->line = cmd;
                    newJob->r_ptr = cmd;
                    newJob->len = strlen(cmd);
                    push_res_job(cur_dw_res->u->jobq, newJob);
                    //cur_dw_res->u->state = UNKNOWN_USR;
                }
                break;
        }
        dispatcher(scxt, sobj, cur_dw_res->u);
        q.pop();
    }
}