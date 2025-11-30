#include "../lib/universal.h"
#include "../lib/command.h"

#include "objects/objects.h"
#include "io/io.h"
#include "dispatcher/dispatcher.h"
#include "logic/logic.h"
#include "parser/parser.h"

#include <assert.h>

typedef enum{
    ACT_SEND,
    DB_LOGIN,
    DB_SIGNUP,
} Action_t;

void Epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);

void Socketpair(int domain, int type, int protocol, int sv[1]);

conn_t *get_user(int fd, std::map<int, conn_t*> &fd_to_conn);

pid_t Fork();

void Close(int fd);

void Execv(const char *path, char *const argv[]);