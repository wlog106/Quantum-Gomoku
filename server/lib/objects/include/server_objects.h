#ifndef SERVER_OBJECTS_H
#define SERVER_OBJECTS_H

#include <share_wrap.h>
#include <string>
#include <ctime>
#include <queue>
#include <array>
#include <set>
#include <map>
#include <sys/time.h>

struct conn;
struct db_conn;
struct Room;
struct Board;
struct job_t;
struct linear_buf_t;
struct Uid_generator;

struct ServerContext{
    int epfd;
    int cur_fd;
    int listenfd;
    int dw_fd;
    ServerContext();
    ServerContext(
        int epfd, 
        int listenfd,
        int dw_fd
    );
};

struct ServerObjects{
    std::queue<job_t*> *dwq;
    linear_buf_t *dwr_buf;
    Uid_generator *uid_gen;
    std::map<int, conn*> *fd_to_conn;
    std::set<int> *playing_room_fds;
    std::map<std::string, Room*> *id_to_room;
    ServerObjects();
    ServerObjects(
        std::queue<job_t*> *dwq,
        linear_buf_t *dwr_buf,
        Uid_generator *uid_gen,
        std::map<int, conn*> *fd_to_conn,
        std::set<int> *playing_room_fds,
        std::map<std::string, Room*> *id_to_room
    );
};

class linear_buf_t{
    std::string buf;
    int cap;
    int read_pos;
public:
    linear_buf_t();
    linear_buf_t(int cap);
    void append(char *data);
    void append(std::string &data);
    void set_buf(std::string &&data);
    char *getline();
};

struct job_t{
    int type;
    char *line;
    char *r_ptr;
    int len;
    void fill_line(char *cmd);
};

struct conn{
    /* connection information */
    int id;
    int fd;
    char name[65];
    char hash[65];
    int cur_elo;
    int state;
    std::string room_id;

    /* connection buffer */
    linear_buf_t *r_buf;
    std::deque<job_t*> jobq;

    /* constructor */
    conn();
    conn(int fd, int state);

    ~conn();

    /* for observe result */
    int tfd;
    struct itimerspec start, stop;
    void start_timer();
    void stop_timer();
};

struct dw_res{
    conn *u;
    int type;
    int result;
};

struct job_res {
    char *line;
    char *r_ptr;
    int len;
};

struct DwContext{
    int mainfd, epfd;
    db_conn *dc;
    std::queue<job_res> *resultq;
    linear_buf_t *stream_buf;
    bool have_pending_res;
    std::queue<job_t> *jobq;
    DwContext();
    DwContext(
        int mainfd, 
        int epfd,
        db_conn *dc, 
        std::queue<job_res> *resultq,
        linear_buf_t *stream_buf,
        std::queue<job_t> *jobq
    );
};

struct Room{
    std::string room_id;
    conn *users[5];
    bool is_playing;
    int room_fd;
    bool user_existance[5];
    bool user_ready[2];
    Room();
    Room(std::string rm_id);
    bool add_user(conn* new_user);
    bool add_player(conn* new_user);
    bool add_observer(conn* new_user);
    bool user_leave(conn* leaving_user);
    bool user_change_position(conn *u, int pos);
    bool change_ready(conn *u);
    bool can_fork();
    bool is_empty();
    void on_change(int epfd, conn *u);
    void broadcast_msg(int epfd, conn *u, char *msg);
    std::string get_room_info();
    std::string get_exist_usernames();
    std::string get_exist_userfds();
    void turn_off_fd_close_on_exec();
    void erase_exist_userinfo(
        ServerObjects *sobj,
        int epfd
    );
};

struct Game{
    int epfd;
    int mainfd;
    char room_id[10];
    bool cur_player;
    bool game_terminate;
    bool observed_flag;
    long long p1_time;
    long long p2_time;
    long long last_seg_start;
    bool user_exist[5];
    conn *users[5];
    Board *board;
    Game(int epfd);
    ~Game();
    conn *get_user(int fd);
    conn *get_exp_user(int tfd);
    int get_pos(conn *u);
    int get_pos(int fd);
    conn *get_empty_observe_pos();
    std::string board_to_string(
        std::vector<std::vector<int>> &v
    );
    std::string get_full_game_info();
    std::string get_player_info();
    void broadcast_init_msg();
    void broadcast_game_result(int result);
    void broadcast_observe_result(
        std::vector<std::vector<int>> &v
    );
    void broadcast_msg(char *msg, int type);
    void reset_timer();
    void align_timer();
    void start_next_seg(int pos_x, int pos_y, int type);
    void do_observe(int pos_x, int pos_y, int type);
    void calculate_new_elo(
        std::pair<int, int> &p, 
        int result
    );
    void delete_user(conn *u);

    void pass_ufd_to_main(conn *u);

};

class Uid_generator{
    std::set<std::string> distributed_uid;
    std::string character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
public:
    Uid_generator();
    std::string new_uid(int len);
    bool rm_uid(std::string uid);
};

#endif