#ifndef LOGIC_H
#define LOGIC_H

#include <queue>
#include <string>

struct ServerContext;
struct conn_t;

void processor(
    ServerContext *scxt, 
    conn_t *u, 
    std::queue<std::string> &q
);

#endif