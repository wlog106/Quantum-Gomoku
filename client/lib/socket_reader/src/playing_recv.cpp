#include "../socket_reader.h"

void playing_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id){
        case C_playing_new_segement:
            lock_ui();
            if(playing_page_type != 1){
                playing_page_type = 1;
                PP_initialize();
            }
            read_segment_info(ss);
            PP_segement_start();
            unlock_ui();
            break;
        case C_start_observing:
            lock_ui();
            playing_page_type = 2;
            PP_observe(ss);
            unlock_ui();
            break;
        case C_game_over:
            lock_ui();
            game_over = 1;
            PP_show_playing_result(ss);
            unlock_ui();
            break;
        case C_playing_users_change:
            lock_ui();
            PP_user_change(ss);
            unlock_ui();
            break;
        default:
            break;
    }
}

void read_playing_info(stringstream &ss){
    ss >> playing_room_id;
    for(int i = 0; i < 5; i++){
        ss >> playing_user_existance[i];
    }
    for(int i = 0; i < 5; i++){
        if(playing_user_existance[i]){
            ss >> playing_username[i];
        }
    }
    for(int i = 0; i < Board_size; i++){
        for(int j = 0; j < Board_size; j++){
            ss >> playing_board.board_data[i][j];
        }
    }
    ss >> playing_position;
    if(playing_position == 1) my_piece_type = 9;
    else if(playing_position == 2) my_piece_type = 1;
}

void read_segment_info(stringstream &ss){
    int has_new_piece;
    ss >> has_new_piece;
    if(has_new_piece){
        int x, y, type;
        ss >> x >> y >> type;
        PP_drop_piece(x, y, type);
    }
    ss >> player_remain_time[0] >> player_remain_time[1];
    ss >> moving_position;
}