#include <server_utils.h>
#include <server_objects.h>
#include <server_cmd.h>
#include <share_cmd.h>

void pr_leaving_dispatcher(
    Game *g,
    conn *u,
    int cmd
){
    if(cmd == LEAVE_GAME){
        if(u==g->users[0])
            u->cur_elo = g->new_elo.first;
        else if(u==g->users[1])
            u->cur_elo = g->new_elo.second;
        g->pass_ufd_to_main(u, 0);
        g->delete_user(u);
        /* observer leave */
        if(!g->game_terminate){
            char msg[40];
            sprintf(msg, "%d %s\n",
                    C_playing_users_change, g->get_player_info().data());
            g->broadcast_msg(msg, RES_USR);
        }
    }
    else{
        if(u==g->users[0]){
            /* player 1 leave player 2 win*/
            printf("(%s) player 1 %s die...\n", g->room_id, u->name);
            g->broadcast_game_result(2);
            u->cur_elo = g->new_elo.first;
            g->pass_ufd_to_main(u, 1);
            g->delete_user(u);
        }
        else if(u==g->users[1]){
            printf("(%s) player 2 %s die...\n", g->room_id, u->name);
            g->broadcast_game_result(1);
            u->cur_elo = g->new_elo.second;
            g->pass_ufd_to_main(u, 1);
            g->delete_user(u);
        }
        else{
            printf("(%s) observer %s die...\n", g->room_id, u->name);
            g->pass_ufd_to_main(u, 1);
            g->delete_user(u);
            g->broadcast_player_change();
        }
    }
}