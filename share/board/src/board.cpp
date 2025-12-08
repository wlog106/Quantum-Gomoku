#include <share_board.h>

Board::Board(int board_size):size(board_size){
    board_data.resize(size, std::vector(size, 0));
}

int Board::set_piece(int x, int y, int piece_type){
    if(x < 0 || x >= size) return -1;
    if(y < 0 || y >= size) return -1;
    if(piece_type != 0 || 
       piece_type != 1 ||
       piece_type != 3 ||
       piece_type != 7 ||
       piece_type != 9){
        return -1;
    }
    board_data[x][y] = piece_type;
    return 0;
}