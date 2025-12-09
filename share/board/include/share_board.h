
#ifndef BOARD_H
#define BOARD_H

#include<vector>

struct Board{
    int size;
    std::vector<std::vector<int>> board_data;
    Board(int);
    int get_observe_result(std::vector<std::vector<int>>&);
};

#endif