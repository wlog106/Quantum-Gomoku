#ifndef PARSER_H
#define PARSER_H

struct ServerContext;
struct conn_t;

void parser(
    ServerContext *scxt,
    conn_t *u
);

#endif