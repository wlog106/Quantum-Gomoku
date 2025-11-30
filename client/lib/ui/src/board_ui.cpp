#include "../ui.h"
#include <ostream>

#define Board_pos_x 1
#define Board_pos_y 1
#define Borad_size 19

/*
┌ ┐ └ ┘┼┬ ┴├ ┤─│
*/

void debug_ui(){
    clear_board_ui();
    return;
}

void clear_board_ui(){
    cout << CLEAR_SCREEN <<  MOVE(Board_pos_x, Board_pos_y) << flush;
    cout << "┌";
    for(int i = 2; i < Borad_size; i++)cout << "─┬";
    cout << "─┐" << flush;
}