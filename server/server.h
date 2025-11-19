#include "../lib/universal.h"
#include <sys/epoll.h>

#define MAX_EVENT 10

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
int Epoll_create();
void Epoll_ctl_add(int epollfd, int sockfd, struct epoll_event *ev);
int Epoll_wait(int epollfd, struct epoll_event *events, int maxevents);
void Listen(int fd, int backlog);
int Init_listenfd(struct sockaddr_in *servaddr, socklen_t salen);
Sigfunc *Signal(int signo, Sigfunc *sighandler);
void sigchild(int signo);