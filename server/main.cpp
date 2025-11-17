#include "../lib/universal.h"
#include <csignal>
#include <sys/select.h>
using std::map;
using std::string;

int main(int argc, char **argv){
    /* store client data*/
    map<string, int>    sockfd_to_name;
    map<int, string>    name_to_sockfd;
    /* for select */
    int                 i, maxi, maxfd, nready;
    fd_set              rest, allset;
    /* for establish connect */
    int                 listenfd, listenchlid, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;
    /* for signal handler */
    Sigfunc             *kill_zombie;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family       = AF_INET;
    servaddr.sin_addr.s_addr  = htonl(INADDR_ANY);
    servaddr.sin_port         = htons(TEST_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LIS_BACKLOG);

    Signal(SIGCHLD, kill_zombie);

    maxfd = listenfd;
    maxi = -1;

    for( ; ; ){
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

    }
}