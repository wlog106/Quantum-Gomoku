#include "../ui.h"
#include "playing_ui/playing_ui.h"


void PP_initialize(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[1m" << flush;
    cout << "\n                   Quantun Gomoku Playing Room\n";
    cout << "                         Room ID: " << playing_room_id << "\x1b[0m" << flush;
    clear_board_ui();
    for(int i = 0; i < Board_size; i++){
        for(int j = 0; j < Board_size; j++){
            draw_piece(i, j, playing_board.board_data[i][j]);
        }
    }
    show_all_user();
    cout << MOVE(Timer_pos_x, Timer_pos_y - 11) << "Time left:" << flush;
    cout << MOVE(Timer_pos_x+1, Timer_pos_y - 11) << "Time left:" << flush;
}

void PP_refresh_observe_chance_info(){
    cout << MOVE(Observe_chance_pos_x, Observe_chance_pos_y) << "Observe chance: " << observing_chance;
    cout << MOVE(Observe_chance_pos_x+1, Observe_chance_pos_y) << "Press O to observe after this hand." << flush;
}

void PP_segement_start(){
    has_observe = 0;
    cout << MOVE(Observe_chance_pos_x, Observe_chance_pos_y) << "Observe chance: " << observing_chance;
    cout << MOVE(Observe_chance_pos_x+1, Observe_chance_pos_y) << "Press O to observe after this hand." << flush;
    if(moving_position == playing_position) {
        cursor_pos_x = (Board_size - 1) / 2;
        cursor_pos_y = (Board_size - 1) / 2;
        draw_cursor(cursor_pos_x, cursor_pos_y);
    }
    else{
        erase_cursor(cursor_pos_x, cursor_pos_y);
    }
    PP_close_timer();
    set_timer();
}

void PP_move_cursor(int direction){
    erase_cursor(cursor_pos_x, cursor_pos_y);
    draw_piece(cursor_pos_x, cursor_pos_y, playing_board.board_data[cursor_pos_x][cursor_pos_y]);
    switch (direction) {
        case 0: 
            cursor_pos_x--;
            break;
        case 1:
            cursor_pos_y++;
            break;
        case 2:
            cursor_pos_x++;
            break;
        case 3:
            cursor_pos_y--;
            break;
        default:
            break;
    }
    draw_cursor(cursor_pos_x, cursor_pos_y);
}

void PP_no_time(){
    moving_position = 0;
}

void PP_drop_piece(int x, int y, int type){
    playing_board.board_data[x][y] = type;
    draw_piece(x, y, type);
    return;
}

void PP_observe(stringstream &ss){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[1m" << flush;
    cout << "\n                   Quantun Gomoku Playing Room\n";
    cout << "                         Room ID: " << playing_room_id << "\x1b[0m" << flush;
    clear_board_ui();
    int piece_type;
    for(int i = 0; i < Board_size; i++){
        for(int j = 0; j < Board_size; j++){
            ss >> piece_type;
            draw_real_piece(i, j, piece_type);
        }
    }
    show_all_user();
}

void PP_show_playing_result(stringstream &ss){
    string winner;
    int origin_elo, new_elo;
    ss >> winner >> origin_elo >> new_elo;
    cout << MOVE(Game_result_pos_x, Game_result_pos_y);
    //TODO: show result at the bottom
}