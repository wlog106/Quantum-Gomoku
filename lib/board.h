#pragma once
#include<vector>

struct Board{
    int size;
    std::vector<std::vector<int>> board_data;
    Board(int);
    int set_piece(int, int, int);
};