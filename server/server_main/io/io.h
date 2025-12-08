#ifndef IO_H
#define IO_H

#include <map>

struct ServerObjects;
struct ServerContext;
struct conn;

void on_dw_res(
    ServerContext *scxt,
    ServerObjects *sobj
);

void on_listen(
    ServerContext *scxt,
    ServerObjects *sobj
);

void on_readable(
    ServerContext *scxt, 
    ServerObjects *sobj
);

void on_writable(
    ServerContext *scxt,
    ServerObjects *sobj
);

#endif