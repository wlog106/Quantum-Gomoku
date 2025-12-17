#include <server_objects.h>
#include <server_cmd.h>
#include <share_cmd.h>

void pr_leaving_dispatcher(
    Game *g,
    conn *u
){
    if(u==g->users[0]){
        /* player 1 leave player 2 win*/
        g->broadcast_game_result(2);
        /* delete player and go through normal 
        game terminate process*/
    }
    else if(u==g->users[1]){
        g->broadcast_game_result(1);
    }
    else{
        printf("(%s) observer %s die\n", g->room_id, u->name);
        g->delete_user(u);
        char msg[40];
        sprintf(msg, "%d %s\n", C_playing_users_change, g->get_player_info().data());
        g->broadcast_msg(msg, RES_USR);
    }
}