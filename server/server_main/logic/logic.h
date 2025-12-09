#ifndef LOGIC_H
#define LOGIC_H

#include <map>
#include <queue>
#include <string>

struct ServerContext;
struct ServerObjects;
struct conn;
struct dw_res;

void processor(
    ServerContext *scxt, 
    ServerObjects *sobj,
    conn *u, 
    std::queue<std::pair<int, char*>> &q
);

void dw_res_processor(
    ServerContext *scxt,
    ServerObjects *sobj,
    std::queue<dw_res> &q
);

#endif