
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
                iov[0].iov_base = &jobs.front();
                iov[0].iov_len = sizeof(job_t);
                iocnt = sizeof(iov) / sizeof(iovec);
                writev(scxt->dw_fd, iov, iocnt);
        }
    }
}