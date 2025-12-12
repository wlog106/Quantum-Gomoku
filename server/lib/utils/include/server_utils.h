#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include "server_objects.h"
#include <sys/socket.h>
#include <map>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

typedef void Sigfunc(int);
struct conn;
struct ServerContext;

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
#define MAX_JOB 10
#define MAX_EVENT 50

#define evfd events[i].data.fd
#define evtype events[i].events

conn *get_user(
    int fd, 
    std::map<int, conn*> *fd_to_conn
);

void epoll_r_add(
    int epfd,
    int fd
);

void epoll_r_mod(
    int epfd,
    int fd
);

void epoll_rw_mod(
    int epfd,
    int fd
);

pid_t fork_room(
    ServerObjects *sobj,
    Room *room
);

#endif