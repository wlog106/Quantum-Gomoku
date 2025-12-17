#include <queue>
#include <server_objects.h>

#include "../pr_processor/pr_processor.h"

void pr_parser(
    Game *g, 
    conn *u
){
    char *line;
    std::queue<char*> to_be_freed;
    std::queue<std::pair<int, char*>> q;
    while((line = u->r_buf->getline())!=NULL){
        int cmd_id;
        char *space = strchr(line, ' ');
        if(space != NULL) *space++ = 0;
        sscanf(line, "%d", &cmd_id);
        to_be_freed.push(line);
        q.push({cmd_id, space});
    }
    if(!q.empty()) pr_processor(g, u, q);
    while(!to_be_freed.empty()){
        free(to_be_freed.front());
        to_be_freed.pop();
    }
}