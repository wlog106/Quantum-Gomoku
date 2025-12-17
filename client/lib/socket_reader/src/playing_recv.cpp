#include "../socket_reader.h"

void playing_recv(const string &command){
    stringstream ss(command);
    segment_info_t segment_info;
    users_t user_info;
    game_result_t game_result;
    Board observed_board = Board(Board_size);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id){
        case C_playing_new_segement:
            if(read_segment_info(ss, segment_info)) return;
            lock_ui();
            if(playing_page_type != 1){
                playing_page_type = 1;
                PP_initialize();
            }
            apply_segment_info(segment_info);
            PP_segement_start();
            unlock_ui();
            break;
        case C_show_observe_result:
            for(int i = 0; i < Board_size; i++){
                for(int j = 0; j < Board_size; j++){
                    ss >> observed_board.board_data[i][j];
                }
            }
            if(ss.fail())return;
            lock_ui();
            playing_page_type = 2;
            PP_observe(observed_board);
            unlock_ui();
            break;
        case C_game_over:
            ss >> game_result.player_name[0];
            ss >> game_result.origin_elo[0];
            ss >> game_result.new_elo[0];
            ss >> game_result.player_name[1];
            ss >> game_result.origin_elo[1];
            ss >> game_result.new_elo[1];
            ss >> game_result.wining[0];
            ss >> game_result.wining[1];
            if(ss.fail())return;
            lock_ui();
            game_over = 1;
            PP_show_playing_result(game_result);
            unlock_ui();
            break;
        case C_playing_users_change:
            for(int i = 0; i < 5; i++) ss >> user_info.user_existance[i];
            for(int i = 0; i < 5; i++){
                if(user_info.user_existance[i]){
                    ss >> user_info.user_name[i];
                }
            }
            if(ss.fail())return;
            lock_ui();
            PP_user_change(user_info);
            unlock_ui();
            break;
        default:
            break;
    }
}