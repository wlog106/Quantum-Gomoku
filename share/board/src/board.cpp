#include <share_board.h>
#include <random>

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<double> dist(0.0, 1.0);

Board::Board(int board_size):size(board_size){
    board_data.resize(size, std::vector(size, 0));
}

std::string Board::get_board_info(){
    std::string info;
    for(auto &row : board_data){
        for(int &col : row){
            info += std::to_string(col);
            info += " ";
        }
    }
    info.pop_back();
    return info;
}

int Board::get_observe_result(std::vector<std::vector<int>> &observed_board){
    observed_board.resize(size, std::vector<int>(size, 0));
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(board_data[i][j] == 0){
                observed_board[i][j] = 0;
            }
            else{
                double p = board_data[i][j] / 10.0;
                observed_board[i][j] = ((dist(rng) < p) ? 1 : 2);
            }
        }
    }
    auto vaild = [&](int x, int y)->bool {
        if(x < 0 || x >= size)return false;
        if(y < 0 || y >= size)return false;
        return true;
    };
    int dir[4][2] = {{0, 1},{1, 1},{1, 0},{1, -1}};
    bool black_flag = 0;
    bool white_flag = 0;
    for(int type = 1; type <= 2; type++){
        bool full_flag;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                for(int k = 0; k < 4; k++){
                    full_flag = 1;
                    int x = i, y = j;
                    for(int l = 0; l < 5; l++){
                        if(!vaild(x, y) || observed_board[x][y] != type){
                            full_flag = 0;
                            break;
                        }
                    }
                    if(full_flag){
                        if(type == 1)black_flag = 1;
                        else white_flag = 1;
                        break;
                    }
                }
                if(full_flag) break;
            }
            if(full_flag) break;
        }
    }
    if(black_flag && white_flag){
        return 3;
    }
    else if(black_flag){
        return 1;
    }
    else if(white_flag){
        return 2;
    }
    else return 0;
}