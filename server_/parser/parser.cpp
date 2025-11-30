#include "../server_.h"
#include <cstddef>
#include <sstream>


void parser(
    ServerContext *scxt,
    conn_t *u
){
    std::string cmd;
    int cmd_id;
    std::string cmd_line;
    std::stringstream ss;
    while((cmd = u->r_buf.get_line())!=NULL){
        ss >> cmd_id >> cmd_line;
        if(!validator(u, cmd_id, cmd_line)) 
            continue;
        processor(scxt, u, q);
    }
}