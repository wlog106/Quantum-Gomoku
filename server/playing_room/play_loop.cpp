#include <server_utils.h>
#include <server_objects.h>
#include <cstdio>
#include <sstream>
#include <sys/epoll.h>

#include "pr_io/pr_io.h"

int main(int arg, char **argv){
    //raise(SIGSTOP);
    int mainfd, nfds;
    unsigned int exist_pos;
    struct epoll_event events[MAX_EVENT];
    int epfd = Epoll_create();
    Game *game = new Game(epfd);

    sscanf(argv[1], "%d %s %u",
           &mainfd, game->room_id, &exist_pos);
    
    game->mainfd = mainfd;
    epoll_r_add(epfd, mainfd);

    std::stringstream *ssName = new std::stringstream;
    std::stringstream *ssfd = new std::stringstream;
    *ssName << std::string(argv[2]);
    *ssfd << std::string(argv[3]);
    for(int i = 0 ; i < 5; i++){
        if(!(exist_pos & (1 << i)))
            continue;
        *ssName >> game->users[i]->name;
        *ssfd >> game->users[i]->fd;
        game->user_exist[i] = true;
    }
    delete ssName;
    delete ssfd;

    game->broadcast_init_msg();

    for( ; ; ){
        nfds = Epoll_wait(epfd, events, MAX_EVENT);
        for(int i=0; i<nfds; i++){
            if(timer_exp(game, evfd))
                continue;
            if(events[i].events & EPOLLIN){
                if(evfd == mainfd) 
                    on_observer_join(game, evfd);
                else 
                    on_recv(game, evfd);
            }
            else if(events[i].events & EPOLLOUT){
                on_send(game, evfd);
            }
        }
    }
}