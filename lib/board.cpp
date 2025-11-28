#include "board.h"

Board::Board(int board_size):size(board_size){
    board_data.resize(size, std::vector(size, 0));
}

int Board::set_piece(int x, int y, int piece_type){
    if(x < 0 || x >= size) return -1;
    if(y < 0 || y >= size) return -1;
    if(piece_type !=  0 || 
       piece_type != 10 ||
       piece_type != 30 ||
       piece_type != 70 ||
       piece_type != 90){
        return -1;
    }
    board_data[x][y] = piece_type;
    return 0;
}