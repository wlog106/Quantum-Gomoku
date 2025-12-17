#include "../socket_reader.h"


int read_waiting_info(stringstream &ss, waiting_room_t &waiting_info){
    ss >> waiting_info.room_id;
    for(int i = 0; i < 5; i++) ss >> waiting_info.users.user_existance[i];
    for(int i = 0; i < 2; i++) ss >> waiting_info.ready[i];
    if(ss.fail())return 1;
    for(int i = 0; i < 5; i++){
        if(waiting_info.users.user_existance[i]){
            ss >> waiting_info.users.user_name[i];
        }
    }
    if(ss.fail())return 1;
    return 0;
}

int read_playing_info(stringstream &ss, playing_room_t &playing_info){
    ss >> playing_info.room_id;
    for(int i = 0; i < 5; i++) ss >> playing_info.users.user_existance[i];
    if(ss.fail())return 1;
    for(int i = 0; i < 5; i++){
        if(playing_info.users.user_existance[i]){
            ss >> playing_info.users.user_name[i];
        }
    }
    for(int i = 0; i < Board_size; i++){
        for(int j = 0; j < Board_size; j++){
            ss >> playing_info.playing_board.board_data[i][j];
        }
    }
    ss >> playing_info.playing_position;
    if(ss.fail())return 1;
    return 0;
}

int read_segment_info(stringstream &ss, segment_info_t &segment_info){
    ss >> segment_info.has_new_piece;
    if(ss.fail())return 1;
    if(segment_info.has_new_piece){
        ss >> segment_info.x >> segment_info.y >> segment_info.type;
    }
    ss >> segment_info.timer[0] >> segment_info.timer[1];
    ss >> segment_info.moving_position;
    if(ss.fail())return 1;
    return 0;
}

void apply_waiting_info(waiting_room_t &waiting_info){
    waiting_room_id = waiting_info.room_id;
    for(int i = 0; i < 5; i++) waiting_user_existance[i] = waiting_info.users.user_existance[i];
    for(int i = 0; i < 2; i++) waiting_is_ready[i] = waiting_info.ready[i];
    for(int i = 0; i < 5; i++){
        if(waiting_user_existance[i]){
            waiting_username[i] = waiting_info.users.user_name[i];
        }
    }
    return;
}

void apply_playing_info(playing_room_t &playing_info){
    playing_room_id = playing_info.room_id;
    for(int i = 0; i < 5; i++) playing_user_existance[i] = playing_info.users.user_existance[i];
    for(int i = 0; i < 5; i++){
        if(playing_user_existance[i]){
            playing_username[i] = playing_info.users.user_name[i];
        }
    }
    playing_board = playing_info.playing_board;
    playing_position = playing_info.playing_position;
    if(playing_position == 1) my_piece_type = 9;
    else if(playing_position == 2) my_piece_type = 1;
    return;
}

void apply_segment_info(segment_info_t &segment_info){
    if(segment_info.has_new_piece){
        PP_drop_piece(segment_info.x, segment_info.y, segment_info.type);
    }
    player_remain_time[0] = segment_info.timer[0];
    player_remain_time[1] = segment_info.timer[1];
    moving_position = segment_info.moving_position;
    return;
}