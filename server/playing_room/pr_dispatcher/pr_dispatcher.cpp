#include "share_cmd.h"
#include <cstdio>
#include <server_utils.h>
#include <server_objects.h>
#include <server_cmd.h>
#include <share_board.h>

void pr_dispatcher(
    Game *g,
    conn *u
){
    job_t *curJob = new job_t;
    while(!u->jobq.empty()){
        curJob = u->jobq.front();
        if(curJob->type == RES_USR){
            epoll_rw_mod(g->epfd, u->fd);
            break;
        }
        else if(curJob->type == UPDATE_GAME_INFO){
            int pos_x, pos_y, type;
            sscanf(curJob->line, "%d %d %d %lld %lld",
                   &pos_x, &pos_y, &type, &g->p1_time, &g->p2_time);
            g->board->board_data[pos_x][pos_y] = type;
            u->jobq.pop_front();
            if(g->observed_flag) 
                g->do_observe(pos_x, pos_y, type);
            else
                g->start_next_seg(pos_x, pos_y, type);
        }
        else if(curJob->type == F_SET_OBSERVE){
            g->observed_flag = true;
            u->jobq.pop_front();
        }
        else if(curJob->type == RUN_OUT_OF_TIME){
            g->broadcast_game_result((g->cur_player^1)+1);
            u->jobq.pop_front();
        }
        else if(curJob->type == LEAVE_GAME){
            g->pass_ufd_to_main(u);
            char msg[40];
            sprintf(msg, "%d %s\n", C_playing_users_change, g->get_player_info().data());
            g->broadcast_msg(msg, RES_USR);
            /* 
                There's no need to process further job,
                since the user no longer exist.
            */
            return;
        }
        else break;
    }
}