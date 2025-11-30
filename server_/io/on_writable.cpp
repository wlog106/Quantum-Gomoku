#include "../server_.h"

#include <string>
#include <sys/epoll.h>
#include <unistd.h>

void on_writable(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
){
    conn_t *u;
    int n;
    u = get_user(scxt->cur_fd, fd_to_conn);
    std::string buf;
    buf = u->w_buf.write_all();
    n = write(u->fd, buf.c_str(), buf.length());
    if(n!=buf.length()){
        u->w_buf.set_buf(buf.substr(0, n));
    }
    else{
        /* user leave */
        if(u->state==?){
            remove_user(u);
            Epoll_ctl(scxt->epfd, EPOLL_CTL_DEL, scxt->cur_fd, NULL);
        }
        /* user doesn't leave */
        else{
            struct epoll_event ev;
            ev.events = EPOLLET;
            ev.data.fd = scxt->cur_fd;
            Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, scxt->cur_fd, &ev);
            u->w_buf.set_buf("");
        }
    }
}