#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>

#include <vector>
#include <queue>

#define TEST_PORT 9000
#define SA struct sockaddr
#define LIS_BACKLOG 1024

#define MAXLINE 50000

typedef void Sigfunc(int);

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Inet_pton(int family, const char *strptr, void *addrptr);
void Listen(int fd, int backlog);
int Read_commamd(int fd, std::queue<std::string> &buf, std::queue<std::string> &result);
int Socket(int domain, int type, int protocol);