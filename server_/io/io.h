#ifndef IO_H
#define IO_H

#include <map>

struct ServerContext;
struct conn_t;

void on_dw_res(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
);

void on_listen(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
);

void on_readable(
    ServerContext *scxt, 
    std::map<int, conn_t*> &fd_to_conn
);

void on_writable(
    ServerContext *scxt,
    std::map<int, conn_t*> &fd_to_conn
);

#endif