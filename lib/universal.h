#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string.h>

#define cout std::cout
#define TEST_PORT 9000

int Socket(int domain, int type, int protocol);
void Inet_pton(int family, const char *strptr, void *addrptr);