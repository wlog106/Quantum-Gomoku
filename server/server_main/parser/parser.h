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

void dw_res_parser(
    ServerContext *scxt,
    ServerObjects *sobj
);

bool validator(
    int state,
    int cmd_id
);

#endif