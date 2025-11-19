#include "../lib/universal.h"
#include <sys/epoll.h>

#define MAX_EVENT 10

int Epoll_create();
void Epoll_ctl_add(int epollfd, int sockfd, struct epoll_event *ev);
int Epoll_wait(int epollfd, struct epoll_event *events, int maxevents);
int Init_listenfd(struct sockaddr_in *servaddr, socklen_t salen);
void sigchild(int signo);