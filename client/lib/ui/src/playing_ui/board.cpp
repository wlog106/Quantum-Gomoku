#include "playing_ui.h"



/*
┌ ┐ └ ┘┼┬ ┴├ ┤─│
*/



void draw_cursor(int x, int y){
    if(x > 0){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x-1, Board_pos_y + (horizontal_bar + 1) * y);
        cout << "\x1b[1;31m║\x1b[0m" << flush;  
    }
    if(x < Board_size - 1){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x+1, Board_pos_y + (horizontal_bar + 1) * y);
        cout << "\x1b[1;31m║\x1b[0m" << flush;
    }
    if(y > 0){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x, Board_pos_y + (horizontal_bar + 1) * y - 2);
        cout << "\x1b[1;31m══\x1b[0m" << flush;
    }
    if(y < Board_size - 1){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x, Board_pos_y + (horizontal_bar + 1) * y + 1);
        cout << "\x1b[1;31m══\x1b[0m" << flush;
    }
}

void erase_cursor(int x, int y){
    if(x > 0){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x-1, Board_pos_y + (horizontal_bar + 1) * y);
        cout << "\x1b[0m│" << flush;  
    }
    if(x < Board_size - 1){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x+1, Board_pos_y + (horizontal_bar + 1) * y);
        cout << "\x1b[0m│" << flush;
    }
    if(y > 0){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x, Board_pos_y + (horizontal_bar + 1) * y - 2);
        cout << "\x1b[0m──" << flush;
    }
    if(y < Board_size - 1){
        cout << MOVE(Board_pos_x + (vertical_bar + 1) * x, Board_pos_y + (horizontal_bar + 1) * y + 1);
        cout << "\x1b[0m──" << flush;
    }
}

void clear_board_ui(){
    int x = Board_pos_x, y = Board_pos_y;
    cout << MOVE(x, y) << "\x1b[0m" << flush;
    cout << "┌";
    for(int i = 2; i < Board_size; i++){
        for(int j = 0; j < horizontal_bar; j++)cout << "─";
        cout << "┬";
    }
    for(int i = 0; i < horizontal_bar; i++)cout << "─";
    cout << "┐" << endl << MOVE(++x, y);
    for(int i = 2; i < Board_size; i++){
        for(int j = 0; j < vertical_bar; j++){
            cout << "│";
            for(int k = 1; k < Board_size; k++){
                for(int l = 0; l < horizontal_bar; l++)cout << " ";
                cout << "│";
            }
            cout << endl << MOVE(++x, y);
        }
        cout << "├";
        for(int j = 2; j < Board_size; j++){
            for(int k = 0; k < horizontal_bar; k++)cout << "─";
            cout << "┼";
        }
        for(int j = 0; j < horizontal_bar; j++)cout << "─";
        cout << "┤" << endl << MOVE(++x, y);
    }
    for(int i = 0; i < vertical_bar; i++){
        cout << "│";
        for(int j = 1; j < Board_size; j++){
            for(int k = 0; k < horizontal_bar; k++)cout << " ";
            cout << "│";
        }
        cout << endl << MOVE(++x, y);
    }
    cout << "└";
    for(int i = 2; i < Board_size; i++){
        for(int j = 0; j < horizontal_bar; j++)cout << "─";
        cout << "┴";
    }
    for(int i = 0; i < horizontal_bar; i++)cout << "─";
    cout << "┘" << endl << MOVE(++x, y);
}

void draw_piece(int x, int y, int piece_type){
    cout << MOVE(Board_pos_x + (vertical_bar + 1) * x, Board_pos_y + (horizontal_bar + 1) * y - 1);
    switch (piece_type) {
        case 0:
            //cout << "\x1b[0m┼";
            break;
        case 1:
            cout << "\x1b[38;5;0;48;5;255;1m 1 ";
            break;
        case 3:
            cout << "\x1b[38;5;0;48;5;252;1m 3 ";
            break;
        case 7:
            cout << "\x1b[38;5;255;48;5;236;1m 7 ";
            break;
        case 9:
            cout << "\x1b[38;5;255;48;5;0;1m 9 ";
            break;
        default:
            break;
    }
    cout << "\x1b[0m";
}

void draw_real_piece(int x, int y, int piece_type){ // 1 black 2 white
    cout << MOVE(Board_pos_x + (vertical_bar + 1) * x, Board_pos_y + (horizontal_bar + 1) * y - 1);
    switch (piece_type) {
        case 0:
            //cout << "\x1b[0m┼";
            break;
        case 1:
            cout << "\x1b[38;5;0;48;5;255;1m   ";
            break;
        case 2:
            cout << "\x1b[38;5;255;48;5;0;1m   ";
            break;
        default:
            break;
    }
    cout << "\x1b[0m";
}
