#include "../../server_.h"
#include <cerrno>
#include <cstdio>
#include <sys/uio.h>
#include <bits/types/struct_iovec.h>


void on_recv_job(DwContext *dwcxt){
    int n;
    char recvbuf[MAXLINE];
    while(1){
        n = read(dwcxt->mainfd, recvbuf, MAXLINE);
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
        recvbuf[n] = 0;
        dwcxt->stream_buf->append(recvbuf);
    }
}