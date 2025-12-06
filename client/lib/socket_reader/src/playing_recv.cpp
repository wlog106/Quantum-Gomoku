#include "../socket_reader.h"

void playing_recv(const string &command){

}

void read_playing_info(stringstream &ss){
    lock_ui();
    ss >> playing_room_id;
    for(int i = 0; i < Board_size; i++){
        for(int j = 0; j < Board_size; j++){
            ss >> playing_board.board_data[i][j];
        }
    }
    ss >> player_remain_time[0] >> player_remain_time[1];
    ss >> playing_position >> moving_position;
    unlock_ui();
}