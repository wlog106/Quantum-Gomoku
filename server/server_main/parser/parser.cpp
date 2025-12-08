#include <server_objects.h>

#include "../logic/logic.h"

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
    while((line = u->r_buf.getline())!=NULL){
        /* should validate line here, it will be implement later
        if(!validator(u, cmd_id, cmd_line)) 
            continue;
        */
        int cmd_id;
        char *space = strchr(line, ' ');
        *space++ = 0;
        sscanf(line, "%d", &cmd_id);
        to_be_freed.push(line);
        q.push({cmd_id, space});
    }
    if(!q.empty()) processor(scxt, sobj, u, q);
    while(!to_be_freed.empty()){
        free(to_be_freed.front());
        to_be_freed.pop();
    }
}