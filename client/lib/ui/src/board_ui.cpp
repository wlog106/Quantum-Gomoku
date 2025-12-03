#include "../ui.h"

#define Board_pos_x 1
#define Board_pos_y 1
#define Borad_size 13
#define vertical_bar 1
#define horizontal_bar 4

/*
┌ ┐ └ ┘┼┬ ┴├ ┤─│
*/

void debug_ui(){
    clear_board_ui();
    return;
}

void clear_board_ui(){
    int x = Board_pos_x, y = Board_pos_y;
    cout << CLEAR_SCREEN <<  MOVE(x, y) << flush;
    cout << "┌";
    for(int i = 2; i < Borad_size; i++){
        for(int j = 0; j < horizontal_bar; j++)cout << "─";
        cout << "┬";
    }
    for(int i = 0; i < horizontal_bar; i++)cout << "─";
    cout << "┐" << endl << MOVE(++x, y);
    for(int i = 2; i < Borad_size; i++){
        for(int j = 0; j < vertical_bar; j++){
            cout << "│";
            for(int k = 1; k < Borad_size; k++){
                for(int l = 0; l < horizontal_bar; l++)cout << " ";
                cout << "│";
            }
            cout << endl << MOVE(++x, y);
        }
        cout << "├";
        for(int j = 2; j < Borad_size; j++){
            for(int k = 0; k < horizontal_bar; k++)cout << "─";
            cout << "┼";
        }
        for(int j = 0; j < horizontal_bar; j++)cout << "─";
        cout << "┤" << endl << MOVE(++x, y);
    }
    for(int i = 0; i < vertical_bar; i++){
        cout << "│";
        for(int j = 1; j < Borad_size; j++){
            for(int k = 0; k < horizontal_bar; k++)cout << " ";
            cout << "│";
        }
        cout << endl << MOVE(++x, y);
    }
    cout << "└";
    for(int i = 2; i < Borad_size; i++){
        for(int j = 0; j < horizontal_bar; j++)cout << "─";
        cout << "┴";
    }
    for(int i = 0; i < horizontal_bar; i++)cout << "─";
    cout << "┘" << endl << MOVE(++x, y);
}