#include "../../server_.h"

void dw_parser(DwContext *dwcxt){
    int cmd_id;
    char *line;
    char *space;
    std::queue<char *> to_be_freed;
    std::queue<pair<int, char*>> q;
    while(1){
        line = dwcxt->stream_buf->getline();
        if(line == NULL)
            break;
        
        space = strchr(line, ' ');
        *space++ = 0;
        sscanf(line, "%d", &cmd_id);
        to_be_freed.push(line);
        q.push({cmd_id, space});
        if(!q.empty()) dw_processor();
        while(!to_be_freed.empty()){
            free(to_be_freed.front());
            to_be_freed.pop();
        }
    }
}