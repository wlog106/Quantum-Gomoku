#include <universal.h>
#include <server_objects.h>

#include "../parser/parser.h"

#include <cerrno>

void on_dw_res(
    ServerContext *scxt,
    ServerObjects *sobj
){
    /* "fd id type:result" */
    int n;
    char recvline[MAXLINE+1];
    while(1){
        n = read(scxt->dw_fd, recvline, MAXLINE);
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        else if(n == 0){
            /* database worker die */
        }
        recvline[n] = 0;
        sobj->dwr_buf->append(recvline);
    }
    dw_res_parser(scxt, sobj);
}