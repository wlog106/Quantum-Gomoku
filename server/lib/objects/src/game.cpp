#include <server_cmd.h>
#include <server_utils.h>
#include <server_objects.h>
#include <share_board.h>
#include <share_wrap.h>
#include <share_cmd.h>

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <assert.h>



Game::Game(int epfd, void (*foo)(Game*)){
    this->epfd = epfd;
    board = new Board(13);
    on_show_observe_result = foo;
    cur_player = 0;
    game_terminate = 0;
    for(int i=0; i<5; i++){
        users[i] = new conn;
        user_exist[i] = false;
    }
}

Game::~Game(){
    delete board;
    for(int i=0; i<5; i++){
        delete users[i];
    }
}

conn* Game::get_user(int fd){
    int i = 0;
    while(i<5 && users[i]->fd != fd)
        i++;
    if(i != 5) return users[i];
    else assert(1==0);
}

int Game::get_pos(conn *u){
    int i = 0;
    while(i<5 && users[i] != u)
        i++;
    if(i != 5) return i+1;
    else assert(1==0);
}

std::string Game::get_full_game_info(){
    std::string info;
    for(int i=0; i<5; i++){
        info += std::to_string(user_exist[i]);
        info += " ";
    }
    for(int i=0; i<5; i++){
        info += std::string(users[i]->name);
        info += " ";
    }
    info += board->get_board_info();
    return info;
}

void Game::broadcast_init_msg(){
    for(int i=0; i<5; i++){
        if(!user_exist[i])
            continue;
        job_t *newJob = new job_t;
        newJob->type = RES_USR;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        sprintf(cmd, "%d %s %d\n%d 0 6000 6000 %d\n", 
                C_start_a_playing_room, get_full_game_info().data(), i+1,
                C_playing_new_segement,  cur_player+1);
        printf("(child) send init msg: %s\n", cmd);
        newJob->fill_line(cmd);
        users[i]->jobq.push_front(newJob);
        epoll_rw_add(epfd, users[i]->fd);
    }
}

void Game::broadcast_game_result(int result){
    std::pair<int, int> new_elo = {users[0]->cur_elo, users[1]->cur_elo};
    calculate_new_elo(new_elo, result);
    char msg[200];
    sprintf(msg, "%d %s %d %d %s %d %d %d %d",
            C_game_over,
            users[0]->name, users[0]->cur_elo, new_elo.first,
            users[1]->name, users[1]->cur_elo, new_elo.second,
            (result == 1), (result == 2));
    broadcast_msg(msg);
}

void Game::broadcast_msg(char *msg){
    for(int i=0; i<5; i++){
        if(!user_exist[i])
            continue;
        job_t *newJob = new job_t;
        newJob->type = RES_USR;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        sprintf(cmd, "%s", msg);
        newJob->fill_line(cmd);
        users[i]->jobq.pop_front();
        users[i]->jobq.push_front(newJob);
        epoll_rw_mod(epfd, users[i]->fd);
    }
}

void Game::reset_timer(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    last_seg_start = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

long long Game::get_time(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000) - last_seg_start;
}

void Game::start_next_seg(
    int pos_x, 
    int pos_y, 
    int type
){
    char seg_info[100];
    cur_player ^= 1;
    sprintf(seg_info, "%d 1 %d %d %d %lld %lld %d",
            C_playing_new_segement, pos_x, pos_y, type, p1_time, p2_time, cur_player+1);
    broadcast_msg(seg_info);
    reset_timer();
}

void Game::do_observe(
    int pos_x, 
    int pos_y, 
    int type
){
    std::vector<std::vector<int>> b;
    int result = this->board->get_observe_result(b);
    on_show_observe_result(this);
    observed_flag = false;
    if(result == 0){
        start_next_seg(pos_x, pos_y, type);
        return;
    }
    broadcast_game_result(result);
}

void Game::calculate_new_elo(
    std::pair<int, int> &p, 
    int result
){
    double e_a, e_b, q_a, q_b, s_a, s_b, k_a, k_b;
    q_a = pow(10.0, (users[0]->cur_elo)/400.0);
    q_b = pow(10.0, (users[1]->cur_elo)/400.0);
    e_a = q_a/(q_a+q_b);
    e_b = q_b/(q_a+q_b);
    s_a = (result==1) + (double)(result==3)/2.0;
    s_b = (result==2) + (double)(result==3)/2.0;
    k_a = (p.first <= p.second) ? 32 : 16;
    k_b = (p.first >= p.second) ? 32 : 16;
    p.first = p.first + k_a*(s_a-e_a);
    p.second = p.second + k_b*(s_b-e_b);
}