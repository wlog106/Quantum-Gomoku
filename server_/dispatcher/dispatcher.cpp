
#include "dispatcher.h"
#include "../server_.h"
#include <bits/types/struct_iovec.h>
#include <sys/socket.h>
#include <sys/uio.h>

void dispatcher(
    ServerContext *scxt,
    std::queue<job_t> &jobs
){
    struct msghdr msg;
    struct iovec iov[1];
    int iocnt;
    while(!jobs.empty()){
        switch(jobs.front().action)
        {
            case (DB_SIGNUP): case (DB_LOGIN):
                job_t *job_ptr = new job_t(jobs.front());
                iov[0].iov_base = job_ptr;
                iov[0].iov_len = sizeof(job_t);
                iocnt = sizeof(iov) / sizeof(iovec);
                writev(scxt->dw_fd, iov, iocnt);
                break;
            case (RES_SIGNUP_SUCCESS):
                break;
        }
    }
}