#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
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

typedef void Sigfunc(int);

void Inet_pton(int family, const char *strptr, void *addrptr);
int Read_commamd(int fd, std::string &buf, std::queue<std::string> &commands);
int Socket(int domain, int type, int protocol);
void Write(int fd, void *ptr, size_t nbytes);