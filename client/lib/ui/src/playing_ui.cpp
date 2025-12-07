#include "../ui.h"
#include "playing_ui/playing_ui.h"


void PP_initialize(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[1m" << flush;
    cout << "                   Quantun Gomoku Playing Room\n";
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

void PP_segement_start(){
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