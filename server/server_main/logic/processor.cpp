#include <share_wrap.h>
#include <share_cmd.h>
#include <server_objects.h>
#include <server_utils.h>
#include <server_cmd.h>

#include "../dispatcher/dispatcher.h"

#include <sys/epoll.h>

void processor(
    ServerContext *scxt, 
    ServerObjects *sobj,
    conn *u, 
    std::queue<std::pair<int, char*>> &q
){
    while(!q.empty()){
        job_t *newJob = new job_t;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        switch(q.front().first)
        {
            case (C_create_new_account):
                sscanf(q.front().second, "%s %s", u->name, u->hash);
                newJob->type = DW_SIGNUP;
                sprintf(cmd, "%d %d %s\n", DW_SIGNUP, u->fd, q.front().second);
                newJob->line = cmd;
                newJob->r_ptr = cmd;
                newJob->len = strlen(cmd);
                u->jobq.push_back(newJob);
                break;
                
            case (C_login_to_server):
                sscanf(q.front().second, "%s %s", u->name, u->hash);
                newJob->type = DW_LOGIN;
                sprintf(cmd, "%d %d %s\n", DW_LOGIN, u->fd, q.front().second);
                newJob->line = cmd;
                newJob->r_ptr = cmd;
                newJob->len = strlen(cmd);
                u->jobq.push_back(newJob);
                break;
            
            case (C_create_room):
                break;
        }
        q.pop();
    }
    dispatcher(scxt, sobj, u);
}