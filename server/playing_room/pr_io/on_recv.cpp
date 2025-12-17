#include <share_wrap.h>
#include <server_objects.h>

#include "../pr_parser/pr_parser.h"
#include "../pr_dispatcher/pr_dispatcher.h"

void on_recv(Game *g, int fd){
    conn *u;
    int n;
    char recvline[MAXLINE+1];
    u = g->get_user(fd);
    while(1){
        n = read(u->fd, recvline, MAXLINE);
        if(n == -1){
            if(errno == EINTR)
                continue;
            else if(errno == EWOULDBLOCK || errno == EAGAIN)
                break;
        }
        else if(n == 0){
            pr_leaving_dispatcher(g, u);
            return;
        }
        recvline[n] = 0;
        u->r_buf->append(recvline);
    }
    pr_parser(g, u);
}