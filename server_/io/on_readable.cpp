#include "../server_.h"
#include <cerrno>

void on_readable(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
){
    conn_t *u;
    int n;
    char cli_to_serv[MAXLINE+1];
    u = get_user(scxt->cur_fd, fd_to_conn);
    while(1){
        n = read(scxt->cur_fd, cli_to_serv, MAXLINE);
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        else if(n == 0){
            //u->state = 1;
        }
        cli_to_serv[n] = 0;
        u->r_buf.append(cli_to_serv);
    }
    parser(scxt, u);
}