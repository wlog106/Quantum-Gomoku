#include <server_utils.h>
#include <server_objects.h>


bool timer_exp(Game *g, int tfd){
    conn *u = g->get_exp_user(tfd);
    if(u==NULL) return false;
    uint64_t exp;
    read(tfd, &exp, sizeof(exp));
    u->stop_timer();
    epoll_rw_mod(g->epfd, u->fd);
    return true;
}