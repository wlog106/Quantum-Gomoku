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