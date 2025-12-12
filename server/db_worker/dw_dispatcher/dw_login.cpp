#include <server_objects.h>
#include <server_utils.h>
#include <server_cmd.h>
#include <mariadb.h>
#include <share_wrap.h>

#include <sys/epoll.h>

void dw_login(DwContext *dwcxt){
    int job_result;
    int user_id;
    int user_fd;
    int job_type = DW_LOGIN;
    char name[MAXLINE];
    char hash[MAXLINE];
    sscanf(dwcxt->jobq->front().line,
           "%d %s %s",
           &user_fd, name, hash);
    unsigned int n = db_get_hash(
        dwcxt->dc,
        name,
        hash
    );
    if(n == 100){
        /* MYSQL_NO_DATA */
        job_result = DW_ERESULT_USERDNE;
    }
    else if(strcmp(
        dwcxt->dc->res_info->hash, 
        hash) != 0
    ){
        job_result = DW_ERESULT_PWDFAIL;
    }
    else if(n != 0){
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
    if(dwcxt->resultq->empty()){
        epoll_rw_mod(dwcxt->epfd, dwcxt->mainfd);
    }
    dwcxt->resultq->push(newRes);
    dwcxt->jobq->pop();
}