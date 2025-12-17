#include <csignal>
#include <fcntl.h>
#include <sys/types.h>
#include <share_wrap.h>
#include <server_objects.h>
#include <server_utils.h>

#include "io/io.h"

#include <sys/epoll.h>
#include <assert.h>

int main(int arg, char **argv){

    //prefork db worker to avoid fd leakage
    char argv_buf[10];
    int dw_fd[2];
    char dw_proc_name[11] = "db_worker";
    pid_t dw_pid;
    Socketpair(AF_UNIX, SOCK_STREAM, 0, dw_fd);
    if((dw_pid = Fork()) == 0){
        Close(dw_fd[0]);
        int flags = Fcntl(dw_fd[1], F_GETFL, 0);
        Fcntl(dw_fd[1], F_SETFL, flags | O_NONBLOCK);
        sprintf(argv_buf, "%d", dw_fd[1]);
        char *dw_argv[] = {dw_proc_name, argv_buf, NULL};
        Execv("./db_worker", dw_argv);
        assert(1==0);
    }
    Close(dw_fd[1]);
    std::cout << "dw_pid: " << dw_pid << "\n";
    /* epoll */
    int nfds, epfd;
    struct epoll_event ev, events[MAX_EVENT];

    /* connection */
    int flags, listenfd;
    struct sockaddr_in servaddr;

    /* user data */
    Uid_generator uid_gen;
    std::map<int, conn*> fd_to_conn;
    std::set<int> login_ids;
    std::set<int> playing_room_fds;
    std::map<std::string, Room*> id_to_room;

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

    epfd = Epoll_create();

    flags = Fcntl(listenfd, F_GETFL, 0);
    Fcntl(listenfd, F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC);
    flags = Fcntl(dw_fd[0], F_GETFL, 0);
    Fcntl(dw_fd[0], F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC);
    flags = Fcntl(epfd, F_GETFL, 0);
    Fcntl(epfd, F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC);
    
    epoll_r_add(epfd, listenfd);
    epoll_r_add(epfd, *dw_fd);

    Signal(SIGCHLD, sigchild);

    /* pack some fds together */
    ServerContext *scxt = new ServerContext(
        epfd, listenfd, *dw_fd
    );

    ServerObjects *sobj = new ServerObjects(
        &dwq, &dwr_buf, &uid_gen,
        &fd_to_conn, &playing_room_fds, 
        &id_to_room, &login_ids
    );

    for( ; ; ){
        nfds = Epoll_wait(epfd, events, MAX_EVENT);
        for(int i=0; i<nfds; i++){
            if(evfd == listenfd){
                on_listen(scxt, sobj);
                continue;
            }
            if(evfd == *dw_fd && evtype & EPOLLIN){
                on_dw_res(scxt, sobj);
                continue;
            }
            if(evtype & EPOLLIN && is_playing_room(sobj, evfd)){
                scxt->cur_fd = evfd;
                on_room_msg(scxt, sobj);
                continue;
            }
            if(evtype & EPOLLIN){
                scxt->cur_fd = evfd;
                on_readable(scxt, sobj);
                continue;
            }
            if(evtype & EPOLLOUT){
                scxt->cur_fd = evfd;
                on_writable(scxt, sobj);
                continue;
            }
            assert(1==0);
        }
    }
}