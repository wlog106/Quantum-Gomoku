#include "../lib/universal.h"
#include "../lib/command.h"
#include "mariadb_api/mariadb_api.h"
#include <sys/epoll.h>
#include <fcntl.h>
using std::string;

#define MAX_EVENT 10
#define MAX_CLIENT 50

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Client_handler(int clifd, std::string &buf);
int Epoll_create();
void Epoll_ctl_add(int epollfd, int sockfd, struct epoll_event *ev);
int Epoll_wait(int epollfd, struct epoll_event *events, int maxevents);
void Listen(int fd, int backlog);
int Init_listenfd(struct sockaddr_in *servaddr, socklen_t salen);
void set_non_block(int fd);
void set_close_exe(int fd);
Sigfunc *Signal(int signo, Sigfunc *sighandler);
void sigchild(int signo);
void split_cmd(const string &cmd, int *cmd_id, string &cmd_info);

bool Login(db_conn *db_handler, string &str);
bool SignUp(db_conn *db_handler, string &str);