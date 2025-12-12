#include <cstdio>
#include <share_cmd.h>
#include <server_cmd.h>
#include <server_objects.h>

#include "../pr_dispatcher/pr_dispatcher.h"

void pr_processor(
    Game *g,
    conn *u,
    std::queue<std::pair<int, char *>> &q
){
    while(!q.empty()){
        job_t *newJob = new job_t;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        switch (q.front().first)
        {
            case (C_playing_move_finish):
                sprintf(cmd, "%s", q.front().second);
                newJob->type = UPDATE_GAME_INFO;
                newJob->fill_line(cmd);
                u->jobq.push_back(newJob);
                break;

            case (C_want_to_observe):
                newJob->type = F_SET_OBSERVE;
                free(cmd);
                newJob->fill_line(NULL);
                u->jobq.push_back(newJob);
                break;

            case (C_run_out_of_time):
                newJob->type = RUN_OUT_OF_TIME;
                free(cmd);
                newJob->fill_line(NULL);
                u->jobq.push_back(newJob);
                break;
        }
        q.pop();
    }
    pr_dispatcher(g, u);
}