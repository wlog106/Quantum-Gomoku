#include <sys/types.h>
#include <universal.h>
#include <server_objects.h>
#include <server_utils.h>

#include "io/io.h"

#include <sys/epoll.h>
#include <assert.h>

int main(int arg, char **argv){
    /*
        prefork room manager & db worker
        in order to avoid fd leakage
    */
    char argv_buf[10];
    int rmgr_fd[2];
    int dw_fd[2];
    pid_t rmgr_pid;
    pid_t dw_pid;
    Socketpair(AF_UNIX, SOCK_STREAM, 0, rmgr_fd);
    Socketpair(AF_UNIX, SOCK_STREAM, 0, dw_fd);
    if((rmgr_pid = Fork()) == 0){
        Close(dw_fd[0]);
        Close(dw_fd[1]);
        Close(rmgr_fd[0]);
        int flags = Fcntl(rmgr_fd[1], F_GETFL, 0);
        Fcntl(rmgr_fd[1], F_SETFL, flags | O_NONBLOCK);
        sprintf(argv_buf, "%d", rmgr_fd[1]);
        char *rm_argv[] = {argv_buf};
        Execv("./room_mgr", rm_argv);
        assert(1==0);
    }
    if((dw_pid = Fork()) == 0){
        Close(rmgr_fd[0]);
        Close(rmgr_fd[1]);
        Close(dw_fd[0]);
        int flags = Fcntl(dw_fd[1], F_GETFL, 0);
        Fcntl(dw_fd[1], F_SETFL, flags | O_NONBLOCK);
        sprintf(argv_buf, "%d", dw_fd[1]);
        char *dw_argv[] = {argv_buf};
        Execv("./db_worker", dw_argv);
        assert(1==0);
    }
    Close(rmgr_fd[1]);
    Close(dw_fd[1]);

    /* epoll */
    int nfds, epfd;
    struct epoll_event ev, events[MAX_EVENT];

    /* connection */
    int flags, listenfd;
    struct sockaddr_in servaddr;

    /* user data */
    std::map<int, conn*> fd_to_conn;

    /* dw job queue */
    std::queue<job_t*> dwq;
    linear_buf_t dwr_buf(MAXLINE);
    

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(TEST_PORT);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    /* for testing */
    int opt = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		fprintf(stderr, "setsockopt(SO_REUSEADDR) failed");
		close(listenfd);
		exit(1);
	}
    ////////////////

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, 1024);

    flags = Fcntl(listenfd, F_GETFL, 0);
    Fcntl(listenfd, F_SETFL, flags | O_NONBLOCK);

    epfd = Epoll_create();

    ev.events = EPOLLET;
    ev.data.fd = listenfd;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    ev.events = EPOLLET;
    ev.data.fd = *dw_fd;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, *dw_fd, &ev);
    ev.events = EPOLLET;
    ev.data.fd = *rmgr_fd;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, *rmgr_fd, &ev);

    /* pack some fds together */
    ServerContext *scxt = new ServerContext(
        epfd, listenfd, *dw_fd, *rmgr_fd
    );

    ServerObjects *sobj = new ServerObjects(
        &dwq, &dwr_buf, &fd_to_conn
    );

    for( ; ; ){
        nfds = Epoll_wait(epfd, events, MAX_EVENT);
        for(int i=0; i<nfds; i++){
            if(evfd == listenfd){
                on_listen(scxt, sobj);
                continue;
            }
            if(evfd == *dw_fd){
                on_dw_res(scxt, sobj);
                continue;
            }
            if(evfd == *rmgr_fd){
                /* handle rmmgr result*/
                continue;
            }
            if(evtype & EPOLLET){
                scxt->cur_fd = evfd;
                on_readable(scxt, sobj);
                continue;
            }
            if(evtype & EPOLLOUT){
                on_writable(scxt, sobj);
                continue;
            }
            assert(1==0);
        }
    }
}