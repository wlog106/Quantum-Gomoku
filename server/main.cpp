#include "server.h"
#include <csignal>
#include <sys/epoll.h>
#include <sys/select.h>
using std::map;
using std::string;

/* 
    use epoll() to serve all waiting clients (i.e. who isn't in playing states).
    use non-blocking accept to handle ECONNECTABORTED & EWOULDBLOCK.
    set every accepted client sockfd to FD_CLOEXEC.

    *before fork(), parent should 
     1. turn off corresponding sockfds' FD_CLOEXEC option.
     2. call sockpair() to create tunnel for IPC later.

    *after fork(), parent should
     1. reco

    *after fork(), child should
     1. call exec() to close all irrelevent sockfds (they were set to FD_CLOEXEC).
*/

int main(int argc, char **argv){
    /* store client data*/
    map<string, int>    sockfd_to_name;
    map<int, string>    name_to_sockfd;
    /* for epoll*/
    int                 nfds;
    int                 epollfd;
    struct epoll_event  ev, events[MAX_EVENT];
    /* for establish connect */
    int                 listenfd, listenchlid, connfd;
    pid_t               childpid;
    struct sockaddr_in  cliaddr, servaddr;
    socklen_t           clilen = sizeof(cliaddr);

    listenfd = Init_listenfd(&servaddr, sizeof(servaddr));

    epollfd = Epoll_create();

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    Epoll_ctl_add(epollfd, listenfd, &ev);

    Signal(SIGCHLD, sigchild);

    for( ; ; ){
        nfds = Epoll_wait(epollfd, events, MAX_EVENT);
        for(int i=0; i<nfds; i++){
            if(events[i].data.fd == listenfd){
                connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
                set_non_block(connfd);
                set_close_exe(connfd);
            }
            else{
                
            }
        }
    }
}