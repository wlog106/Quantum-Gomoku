#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <queue>

struct ServerContext;
struct job_t;

void dispatcher(
    ServerContext *scxt,
    std::queue<job_t> &jobs
);

#endif