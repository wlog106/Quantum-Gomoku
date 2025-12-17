#include <cstring>
#include <server_cmd.h>
#include <server_utils.h>
#include <server_objects.h>
#include <share_board.h>
#include <share_wrap.h>
#include <share_cmd.h>

#include <cstdlib>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <assert.h>
#include <string>
#include <vector>



Game::Game(int epfd){
    this->epfd = epfd;
    board = new Board(13);
    p1_time = 6000;
    p2_time = 6000;
    game_terminate = 0;
    cur_player = 0;
    for(int i=0; i<5; i++){
        users[i] = new conn;
        user_exist[i] = false;
        epoll_r_add(epfd, users[i]->tfd);
    }
}

Game::~Game(){
    delete board;
    for(int i=0; i<5; i++){
        delete users[i];
    }
}

conn* Game::get_user(int fd){
    for(int i=0; i<5; i++){
        if(users[i]!=NULL && users[i]->fd==fd)
            return users[i];
    }
    return NULL;
}

int Game::get_pos(conn *u){
    int i = 0;
    while(i<5 && users[i] != u)
        i++;
    if(i != 5) return i+1;
    else assert(1==0);
}

int Game::get_pos(int fd){
    int i = 0;
    while(i<5 && users[i]->fd != fd)
        i++;
    if(i != 5) return i+1;
    else assert(1==0);
}

conn *Game::get_empty_observe_pos(){
    for(int i=2; i<5; i++){
        if(user_exist[i])
            continue;
        user_exist[i] = 1;
        return users[i];
    }
    return NULL;
}

std::string Game::get_full_game_info(){
    std::string info;
    for(int i=0; i<5; i++){
        info += std::to_string(user_exist[i]);
        info += " ";
    }
    for(int i=0; i<5; i++){
        if(!user_exist[i])
            continue;
        info += std::string(users[i]->name);
        info += " ";
    }
    info += board_to_string(board->board_data);
    return info;
}

std::string Game::get_player_info(){
    std::string info;
    for(int i=0; i<5; i++){
        info += std::to_string(user_exist[i]);
        info += " ";
    }
    for(int i=0; i<5; i++){
        if(!user_exist[i])
            continue;
        info += std::string(users[i]->name);
        info += " ";
    }
    info.pop_back();
    return info;
}

std::string Game::board_to_string(
    std::vector<std::vector<int>> &v
){
    std::string str;
    for(auto &row : v){
        for(int &piece : row){
            str += std::to_string(piece);
            str += " ";
        }
    }
    str.pop_back();
    return str;
}

void Game::broadcast_init_msg(){
    for(int i=0; i<5; i++){
        if(!user_exist[i])
            continue;
        job_t *newJob = new job_t;
        newJob->type = RES_USR;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        sprintf(cmd, "%d %s %s %d\n%d 0 6000 6000 %d\n", 
                C_start_a_playing_room, this->room_id,
                get_full_game_info().data(), i+1,
                C_playing_new_segement,  cur_player+1);
        reset_timer();
        newJob->fill_line(cmd);
        push_res_job(users[i]->jobq, newJob);
        epoll_rw_add(epfd, users[i]->fd);
    }
}

void Game::broadcast_player_change(){
    char msg[40];
    sprintf(msg, "%d %s\n", C_playing_users_change, get_player_info().data());
    broadcast_msg(msg, RES_USR);
}

void Game::broadcast_observe_result(
    std::vector<std::vector<int>> &v
){
    char msg[MAXLINE];
    sprintf(msg, "%d %s\n",
            C_show_observe_result,
            board_to_string(v).data());
    broadcast_msg(msg, SEND_OBSERVE_RESULT);
}

void Game::broadcast_game_result(int result){
    calculate_new_elo(new_elo, result);
    game_terminate = 1;
    char msg[200];
    sprintf(msg, "%d %s %d %d %s %d %d %d %d\n",
            C_game_over,
            users[0]->name, users[0]->cur_elo, new_elo.first,
            users[1]->name, users[1]->cur_elo, new_elo.second,
            (result == 1), (result == 2));
    broadcast_msg(msg, RES_USR);
}

void Game::broadcast_msg(char *msg, int type){
    for(int i=0; i<5; i++){
        if(!user_exist[i])
            continue;
        job_t *newJob = new job_t;
        newJob->type = type;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        sprintf(cmd, "%s", msg);
        newJob->fill_line(cmd);
        push_res_job(users[i]->jobq, newJob);
        epoll_rw_mod(epfd, users[i]->fd);
    }
}

conn *Game::get_exp_user(int fd){
    for(int i=0; i<5; i++)
        if(users[i]->tfd==fd) 
            return users[i];
    return NULL;
}

void Game::reset_timer(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    last_reset_timer = ts.tv_sec * 10 + ts.tv_nsec / 100000000;
}

void Game::align_timer(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long long pass_time = (ts.tv_sec * 10 + ts.tv_nsec / 100000000) - last_reset_timer;
    reset_timer();
    if(cur_player == 0){
        p1_time -= pass_time;
        if(p1_time < 0) p1_time = 0;
    }
    else{
        p2_time -= pass_time;
        if(p2_time < 0) p2_time = 0;
    }
}

void Game::start_next_seg(
    int pos_x, 
    int pos_y, 
    int type
){
    char seg_info[100];
    cur_player ^= 1;
    sprintf(seg_info, "%d 1 %d %d %d %lld %lld %d\n",
            C_playing_new_segement, pos_x, pos_y, type, p1_time, p2_time, cur_player+1);
    broadcast_msg(seg_info, RES_USR);
    reset_timer();
}

void Game::do_observe(
    int pos_x, 
    int pos_y, 
    int type
){
    std::vector<std::vector<int>> v;
    int result = board->get_observe_result(v);
    broadcast_observe_result(v);
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
    std::cout << "cur0: " << p.first << " cur1: " << p.second << "\n";
    std::cout << "ka: " << k_a << " kb: " << k_b << "\n";
    p.first = p.first + k_a*(s_a-e_a);
    p.second = p.second + k_b*(s_b-e_b);
    std::cout << "cur0: " << p.first << " cur1: " << p.second << "\n";
}

void Game::delete_user(
    conn *u
){
    int i = get_pos(u)-1;
    user_exist[i] = false;
    epoll_del(epfd, u->fd);
    epoll_del(epfd, u->tfd);
    close(u->fd);
    delete u;
    if(all_user_leave())
        exit(0);
    users[i] = new conn;
    epoll_r_add(epfd, users[i]->tfd);
}

void Game::pass_ufd_to_main(
    conn *u,
    int op
){
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    int fd_to_pass[1] = {u->fd};
    char iobuf[200];
    sprintf(iobuf, "%d %s %s %d", op, room_id, u->name, u->cur_elo);
    printf("(%s) iobuf: %s\n", room_id, iobuf);
    struct iovec iov = {
        iobuf,
        sizeof(iobuf)
    };

    union {
        char buf[CMSG_SPACE(sizeof(fd_to_pass))];
        struct cmsghdr align;
    } uunn;

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = uunn.buf;
    msg.msg_controllen = sizeof(uunn.buf);
    
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_pass));
    memcpy(CMSG_DATA(cmsg), fd_to_pass, sizeof(fd_to_pass));

    /* only check EINTR signal */
    int n = TEMP_FAILURE_RETRY(sendmsg(mainfd, &msg, 0));
    if(n == -1) {
        printf("(%s) error on passing fd: %d, errno: %d\nno change has been made...\n", 
               room_id, *fd_to_pass, errno);
        return;
    }
    else{
        printf("(%s) send user: %s fd: %d to main loop successfully!\n", 
               room_id, u->name, *fd_to_pass);
    }
}

bool Game::all_user_leave(){
    bool flag = 0;
    for(int i=0; i<5; i++)
        flag |= user_exist[i];
    return !flag;
}