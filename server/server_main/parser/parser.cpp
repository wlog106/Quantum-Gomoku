#include <server_objects.h>
#include <server_utils.h>

#include "../logic/logic.h"
#include "parser.h"

#include <cstring>
#include <cstdio>
#include <queue>


void parser(
    ServerContext *scxt,
    ServerObjects *sobj,
    conn *u
){
    char *line;
    std::queue<char *> to_be_freed;
    std::queue<std::pair<int, char *>> q;
again:
    while((line = u->r_buf->getline())!=NULL){
        int cmd_id;
        char *space = strchr(line, ' ');
        if(space != NULL) *space++ = 0;
        sscanf(line, "%d", &cmd_id);
        to_be_freed.push(line);
        if(!validator(u->state, cmd_id))
            goto again;
        q.push({cmd_id, space});
    }
    if(!q.empty()) processor(scxt, sobj, u, q);
    while(!to_be_freed.empty()){
        free(to_be_freed.front());
        to_be_freed.pop();
    }
}