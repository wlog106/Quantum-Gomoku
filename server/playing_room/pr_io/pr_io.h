#ifndef PR_IO_H
#define PR_IO_H

struct Game;
struct conn;

void on_recv(
    Game *g, 
    int fd
);

void on_send(
    Game *g,
    int fd
);

#endif