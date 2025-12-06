#ifndef PARSER_H
#define PARSER_H

struct ServerContext;
struct ServerObjects;
struct conn;

void parser(
    ServerContext *scxt,
    ServerObjects *sobj,
    conn *u
);

#endif