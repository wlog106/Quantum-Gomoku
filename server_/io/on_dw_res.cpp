#include "../server_.h"
#include <bits/types/struct_iovec.h>
#include <sys/uio.h>

void on_dw_res(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
){
    struct iovec iov[1];
    int iocnt, n;
    Dw_response_t *dw_res;
    while(1){
        n = readv(scxt->dw_fd, iov, MAX_JOB);
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        if(n == 0){
            /* database worker dead */
        }
        else{
            dw_res = (Dw_response_t*)iov[0].iov_base;
        }
    }
    dw_processor(scxt, fd_to_conn, dw_res);
}