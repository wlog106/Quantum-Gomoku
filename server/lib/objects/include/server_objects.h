#ifndef SERVER_OBJECTS_H
#define SERVER_OBJECTS_H

#include <share_wrap.h>
#include <string>
#include <ctime>
#include <queue>
#include <array>
#include <set>
#include <map>

struct conn;
struct db_conn;
struct Room;
struct job_t;
struct linear_buf_t;
struct Uid_generator;

struct ServerContext{
    int epfd;
    int cur_fd;
    int listenfd;
    int dw_fd;
    int rmgr_fd;
    ServerContext();
    ServerContext(
        int epfd, 
        int listenfd,
        int dw_fd,
        int rmgr_fd
    );
};

struct ServerObjects{
    std::queue<job_t*> *dwq;
    linear_buf_t *dwr_buf;
    Uid_generator *uid_gen;
    std::map<int, conn*> *fd_to_conn;
    std::map<std::string, Room*> *id_to_room;
    ServerObjects();
    ServerObjects(
        std::queue<job_t*> *dwq,
        linear_buf_t *dwr_buf,
        Uid_generator *uid_gen,
        std::map<int, conn*> *fd_to_conn,
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

    /* connection metadata */
    int state;
    linear_buf_t *r_buf;
    std::deque<job_t*> jobq; // job queue (except db job)

    /* constructor */
    conn();
    conn(int fd, int state);

    ~conn();
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
    bool user_existance[5];
    bool user_ready[2];
    Room(std::string rm_id);
    bool add_user(conn* new_user);
    bool add_player(conn* new_user);
    bool add_observer(conn* new_user);
    bool user_leave(conn* leaving_user);
    bool user_change_position(conn *u, int pos);
    bool change_ready(conn *u);
    bool can_fork();
    void on_change(ServerContext *scxt, conn *u);
    void broadcast_msg(ServerContext *scxt, conn *u, char *msg);
    std::string get_room_info();
};

class Uid_generator{
    std::set<std::string> distributed_uid;
    string character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
public:
    Uid_generator();
    std::string new_uid(int len);
    bool rm_uid(std::string uid);
};

#endif