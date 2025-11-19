#include "server.h"
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using std::cout;

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr){
    int n;

again:
    if( (n = TEMP_FAILURE_RETRY(accept(fd, sa, salenptr))) < 0 ){
        // ECONNABORTED: client abort before accept
#ifdef EPROTO
        if(errno == EPROTO || errno == ECONNABORTED)
#else
        if(errno == ECONNABORTED)
#endif     
            goto again;
        else{
            cout << "accept error\n";
            exit(1);
        }
    }
    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen){
    if(TEMP_FAILURE_RETRY(bind(fd, sa, salen)) < 0){
        cout << "bind error\n";
        exit(1);
    }
}

int Epoll_create(){
    int fd;
    if((fd = epoll_create1(0)) < 0){
        cout << "fail to create an epoll instance\n";
        exit(1);
    }
    return fd;
}

void Epoll_ctl_add(int epollfd, int sockfd, struct epoll_event *ev){
    if((epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, ev)) < 0){
        cout << "fail to add " << sockfd << " to epoll.\n";
        exit(1);
    }
}

int Epoll_wait(int epollfd, struct epoll_event *events, int maxevents){
    int n;
    if((n = TEMP_FAILURE_RETRY(epoll_wait(epollfd, events, maxevents, -1))) < 0){
        cout << "epoll wait error\n";
        exit(1);
    }
    return n;
}

void Listen(int fd, int backlog){
    if(TEMP_FAILURE_RETRY(listen(fd, backlog)) < 0){
        cout << "listen error\n";
        exit(1);
    }
}

int Init_listenfd(struct sockaddr_in *servaddr, socklen_t salen){

    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(servaddr, salen);
    servaddr->sin_family       = AF_INET;
    servaddr->sin_addr.s_addr  = htonl(INADDR_ANY);
    servaddr->sin_port         = htons(TEST_PORT);

    Bind(listenfd, (SA *)servaddr, salen);

    Listen(listenfd, LIS_BACKLOG);

    return listenfd;
}

Sigfunc *Signal(int signo, Sigfunc *sighandler){
    struct sigaction  action, original_action;
    action.sa_handler = sighandler;
    sigemptyset(&action.sa_mask);  /* don't mask other signal */
    action.sa_flags |= SA_RESTART;
    if(sigaction(signo, &action, &original_action) < 0){
        cout << "fail to enroll signal handler\n";
        exit(1);
    }
    return original_action.sa_handler;
}

void sigchild(int signo){
    pid_t pid;
    int stat;

    while((pid=waitpid(-1, &stat, WNOHANG)) > 0){
        cout << "room with pid: " << pid << "terminated\n";
    }
    return;
}