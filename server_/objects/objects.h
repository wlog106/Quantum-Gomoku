#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <queue>
#include <map>

struct conn;
struct job_t;
struct linear_buf_t;

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
    std::map<int, conn*> *fd_to_conn;
    linear_buf_t *dwr_buf;
    ServerObjects();
    ServerObjects(
        std::queue<job_t*> *dwq,
        linear_buf_t *dwr_buf,
        std::map<int, conn*> *fd_to_conn
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
};

struct conn{
    /* connection information */
    int id;
    int fd;
    char name[65];
    char hash[65];

    /* connection metadata */
    int state;
    bool db_job_pending;
    linear_buf_t r_buf;
    std::queue<job_t*> jobq; // job queue (except db job)

    /* constructor */
    conn();
    conn(int fd, int state);
};

struct dw_res{
    conn *u;
    int type;
    int result;
};

#endif