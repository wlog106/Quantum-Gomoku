#ifndef DISPATCHER_H
#define DISPATCHER_H

struct ServerContext;
struct ServerObjects;
struct conn;

void dispatcher(
    ServerContext *scxt,
    ServerObjects *sobj,
    conn *u
);

#endif