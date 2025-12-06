#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <queue>

struct ServerContext;
struct dw_res;
struct job_t;

void dispatcher(
    ServerContext *scxt,
    std::queue<dw_res> &jobs
);

#endif