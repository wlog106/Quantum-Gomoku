#include <string>

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
    std::string get_line();
    std::string write_all();
};

struct conn_t{
    int id, fd;
    std::string name;
    char hash[65];
    int state;
    linear_buf_t r_buf;
    linear_buf_t w_buf;
    conn_t();
    conn_t(int fd);
};

struct job_t{
    int action;
    conn_t *user;
    job_t();
    job_t(int action, conn_t *user);
};