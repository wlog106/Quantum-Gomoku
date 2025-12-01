#include "../../server_.h"
#include <cerrno>
#include <sys/uio.h>
#include <bits/types/struct_iovec.h>


void on_recv_job(DwContext *dwcxt){
    int n;
    struct iovec iov[MAX_JOB];
    while(1){
        n = readv(dwcxt->mainfd, iov, MAX_JOB);
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        else if(n == 0){
            /* 
                no more job for db_worker,
                kill itself
            */
            exit(1);
        }
        dw_dispatcher(n, iov, dwcxt);
    }
}