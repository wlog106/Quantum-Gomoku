#include <universal.h>
#include <server_objects.h>
#include <server_cmd.h>
#include <mariadb.h>
#include <cstdio>

void dw_signup(DwContext *dwcxt){
    int job_result;
    int user_id;
    int user_fd;
    int job_type = DW_SIGNUP;
    char name[MAXLINE];
    char hash[MAXLINE];
    sscanf(dwcxt->jobq->front().line, 
           "%d %s %s", 
           &user_fd, name, hash);
    unsigned int n = db_add_user(
        dwcxt->dc, 
        name,
        hash
    );
    if(n == 1062){
        job_result = DW_ERESULT_DUPNAME;
    }
    if(n != 0){
        /* unknown error */
        exit(1);
    }
    else{
        job_result = DW_RESULT_SUCCESS;
        int e =  db_get_id_by_name(
            dwcxt->dc,
            name
        );
        if(e != 0 || dwcxt->dc->res_info->id_is_null){
            /* unknown error */
            exit(1);
        }
        else{
            user_id = dwcxt->dc->res_info->id;
        }
    }
    job_res newRes;
    newRes.line = (char*)malloc(MAXLINE*sizeof(char));
    /* fd, id, type:result */
    sprintf(newRes.line, "%d %d %d:%d\n",
            user_fd, user_id, job_type, job_result);
    newRes.r_ptr = newRes.line;
    newRes.len = strlen(newRes.line);
    dwcxt->resultq->push(newRes);
    dwcxt->jobq->pop();
}