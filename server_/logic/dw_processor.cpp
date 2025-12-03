#include "../server_.h"

void dw_processor(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn,
    Dw_response_t *dw_res
){
    std::queue<job_t> jobs;
    switch(dw_res->dw_job)
    {
        case (DW_JOB_SIGNUP):
            if(dw_res->dw_result == DW_RESULT_SUCCESS){
                jobs.push(job_t(RES_SIGNUP_SUCCESS, dw_res->user));
            }
            else if(dw_res->dw_result == DW_ERESULT_DUPNAME){
                jobs.push(job_t(RES_SIGNUP_DUPNAME, dw_res->user));
            }
            else{
                /* unknown error */
            }
            break;
        case (DW_JOB_LOGIN):
            if(dw_res->dw_result == DW_RESULT_SUCCESS){
                jobs.push(job_t(RES_LOGIN_SUCCESS, dw_res->user));
            }
            else if(dw_res->dw_result == DW_ERESULT_USERDNE){
                jobs.push(job_t(RES_LOGIN_USERDNE, dw_res->user));
            }
            else if(dw_res->dw_result == DW_ERESULT_PWDFAIL){
                jobs.push(job_t(RES_LOGIN_PWDFAIL, dw_res->user));
            }
            else{
                /* unknown error */
            }
            break;
    }
    dispatcher(scxt, jobs);
}