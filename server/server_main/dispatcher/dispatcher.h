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

void leaving_dispatcher(
    ServerObjects *sobj,
    conn *u,
    int cmd
);

void room_msg_dispatcher(
    ServerContext *scxt,
    ServerObjects *sobj,
    char *room_msg,
    int fd
);

#endif