#include <share_wrap.h>
#include <server_objects.h>
#include <server_utils.h>

#include "../parser/parser.h"

#include <cerrno>

void on_readable(
    ServerContext *scxt,
    ServerObjects *sobj
){
    conn *u;
    int n;
    char recvline[MAXLINE+1];
    u = get_user(scxt->cur_fd, sobj->fd_to_conn);
    while(1){
        n = read(scxt->cur_fd, recvline, MAXLINE);
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        else if(n == 0){
            /* user leave */
            sobj->fd_to_conn->erase(u->fd);
            Close(u->fd);
            delete u;
            return;
        }
        recvline[n] = 0;
        u->r_buf->append(recvline);
    }
    parser(scxt, sobj, u);
}