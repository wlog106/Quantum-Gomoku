#include <server_utils.h>

#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>

void Bind(
    int fd, 
    const struct sockaddr *sa, 
    socklen_t salen
){
    if(TEMP_FAILURE_RETRY(bind(fd, sa, salen)) < 0){
        fprintf(stderr, "bind error\n");
        exit(1);
    }
}

void Close(int fd){
    if(TEMP_FAILURE_RETRY(close(fd)) < 0){
        fprintf(stderr, "close %d fail\n", fd);
        exit(1);
    }
}

int Epoll_create(){
    int fd;
    if((fd = epoll_create1(0)) < 0){
        fprintf(stderr, "fail to create an epoll instance\n") ;
        exit(1);
    }
    return fd;
}

void Epoll_ctl(
    int epfd, 
    int op, 
    int fd, 
    struct epoll_event *ev
){
    if(epoll_ctl(epfd, op, fd, ev) < 0){
        fprintf(stderr, "Error: epoll_ctl\n");
        exit(1);
    }
}

int Epoll_wait(int epfd, struct epoll_event *events, int maxevents){
    int n;
    if((n = TEMP_FAILURE_RETRY(
        epoll_wait(epfd, events, maxevents, -1))) < 0)
    {
        fprintf(stderr, "epoll wait error\n");
        exit(1);
    }
    return n;
}

void Execv(const char *path, char *const argv[]){
    if(execv(path, argv)==-1){
        int errnum = errno;
        fprintf(stderr, "execv error on path %s, with error number: %d\n",
                path, errnum);
        exit(1);
    }
}

pid_t Fork(){
    pid_t pid = fork();
    if(pid==-1){
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    return pid;
}

void Listen(int fd, int backlog){
    if(TEMP_FAILURE_RETRY(listen(fd, backlog)) < 0){
        fprintf(stderr, "listen error\n");
        exit(1);
    }
}

void Socketpair(int domain, int type, int protocol, int sv[2]){
    if(socketpair(domain, type, protocol, sv) == -1){
        fprintf(stderr, "create sockpair fail\n");
        exit(1);
    }
}

Sigfunc *Signal(int signo, Sigfunc *sighandler){
    struct sigaction  action, original_action;
    action.sa_handler = sighandler;
    sigemptyset(&action.sa_mask);  /* don't mask other signal */
    action.sa_flags |= SA_RESTART;
    if(sigaction(signo, &action, &original_action) < 0){
        fprintf(stderr, "fail to enroll signal handler\n");
        exit(1);
    }
    return original_action.sa_handler;
}