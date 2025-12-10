#include <cassert>
#include <cstddef>
#include <cstdio>
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
            // login
            case (C_create_new_account):
                sscanf(q.front().second, "%s %s", u->name, u->hash);
                newJob->type = DW_SIGNUP;
                sprintf(cmd, "%d %d %s\n", DW_SIGNUP, u->fd, q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;
                
            case (C_login_to_server):
                sscanf(q.front().second, "%s %s", u->name, u->hash);
                newJob->type = DW_LOGIN;
                sprintf(cmd, "%d %d %s\n", DW_LOGIN, u->fd, q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;
            
            // opselect
            case (C_create_room):
                newJob->type = CREATE_ROOM;
                newJob->fill_line(NULL);
                u->jobq.push_back(newJob);
                break;

            case (C_join_room_by_id):
                newJob->type = JOIN_ROOM_BY_ID;
                sprintf(cmd, "%s", q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;
            
            case (C_pair_randomly):
                newJob->type = PAIR_RANDOMLY;
                free(cmd);
                newJob->fill_line(NULL);
                u->jobq.push_back(newJob);
                break;

            case (C_observe_randomly):
                assert(1==0);
                newJob->type = OBSERVE_RANDOMLY;
                free(cmd);
                newJob->fill_line(NULL);
                u->jobq.push_back(newJob);
                break;

            // waiting room
            case (C_change_ready):
                newJob->type = TOGGLE_READY;
                sprintf(cmd, "%s", q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;

            case (C_change_waiting_position):
                newJob->type = CHANGE_POS;
                sprintf(cmd, "%s", q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;

            case (C_leave_waiting_room):
                newJob->type = LEAVE_WAITING_ROOM;
                sprintf(cmd, "%s", q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;
            
            case (C_send_message_waiting_room):
                newJob->type = SENDMSG_WAITING_ROOM;
                sprintf(cmd, "%s", q.front().second);
                newJob->fill_line(cmd);
                u->jobq.push_front(newJob);
                break;
        }
        q.pop();
    }
    dispatcher(scxt, sobj, u);
}