#ifndef UTILS_H
#define UTILS_H

#include <sys/socket.h>
#include <map>

typedef void Sigfunc(int);
struct conn;

/*----------bind utility-----------*/
void Bind(
    int fd, 
    const struct sockaddr *sa, 
    socklen_t salen
);

/*---------close utility-----------*/
void Close(int fd);

/*----------epoll utility----------*/
int Epoll_create();

void Epoll_ctl(
    int epfd, 
    int op, 
    int fd, 
    struct epoll_event *ev
);

int Epoll_wait(
    int epfd, 
    struct epoll_event *events, 
    int maxevents);

/*----------exec utility-----------*/
void Execv(const char *path, char *const argv[]);

/*----------fork utility-----------*/
pid_t Fork();

/*---------listen utility----------*/
void Listen(int fd, int backlog);

/*---------socket utility----------*/
void Socketpair(
    int domain, 
    int type, 
    int protocol, 
    int sv[2]
);

/*---------signal utility----------*/
Sigfunc *Signal(
    int signo, 
    Sigfunc *sighandler
);

void sigchild(int signo);

/*-----handy functions utility-----*/
conn *get_user(
    int fd, 
    std::map<int, conn*> *fd_to_conn
);

#endif