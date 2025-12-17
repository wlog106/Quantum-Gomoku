#pragma once
#include "../client.h"

//login
void login_with_account_recv(const string &command);
void creating_account_recv(const string &command);

//opselect
void select_option_recv(const string &command);
void select_enter_room_id_recv(const string &command);


//waiting room
void waiting_room_recv(const string &command);

//playing room
void playing_recv(const string &command);


struct waiting_room_t{
    string room_id;
    users_t users;
    bool ready[2];
};

struct playing_room_t{
    string room_id;
    users_t users;
    Board playing_board = Board(Board_size);
    int playing_position;
};

struct segment_info_t{
    bool has_new_piece;
    int x, y, type;
    int timer[2];
    int moving_position;
};

//stringstream process
int read_waiting_info(stringstream &ss, waiting_room_t &waiting_info);
int read_playing_info(stringstream &ss, playing_room_t &playing_info);
int read_segment_info(stringstream &ss, segment_info_t &segment_info);

void apply_waiting_info(waiting_room_t &waiting_info);
void apply_playing_info(playing_room_t &playing_info);
void apply_segment_info(segment_info_t &segment_info);

/*
void read_room_info(stringstream &ss){
    lock_ui();
    ss >> waiting_room_id;
    for(int i = 0; i < 5; i++) ss >> waiting_user_existance[i];
    for(int i = 0; i < 2; i++) ss >> waiting_is_ready[i];
    for(int i = 0; i < 5; i++){
        if(waiting_user_existance[i]){
            ss >> waiting_username[i];
        }
    }
    unlock_ui();
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

*/