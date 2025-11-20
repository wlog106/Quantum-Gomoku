#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

#include <cerrno>
#include <csignal>

#include <vector>
#include <queue>
#include <map>

#ifndef _GNU_SOURCE
/* ensure TEMP_FAILURE_RETRY macro is defined */
#define _GNU_SOURCE
#endif

#define TEST_PORT 9000
#define SA struct sockaddr
#define LIS_BACKLOG 1024

#define MAXLINE 50000

using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::queue;
using std::max;
using std::min;

typedef void Sigfunc(int);

void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Fcntl(int fd, int cmd, int arg);
void Inet_pton(int family, const char *strptr, void *addrptr);
int Read_commamd(int fd, std::string &buf, std::queue<std::string> &commands);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
void Shutdown(int fd, int how);
int Socket(int domain, int type, int protocol);
void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void * (*func)(void *), void *arg);
void Pthread_cond_signal(pthread_cond_t *cptr);
void Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
void Pthread_join(pthread_t tid, void **status);
void Pthread_mutex_lock(pthread_mutex_t *mptr);
void Pthread_mutex_unlock(pthread_mutex_t *mptr);
void Write(int fd, void *ptr, size_t nbytes);