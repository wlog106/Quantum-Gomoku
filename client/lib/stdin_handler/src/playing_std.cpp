# include "../stdin_handler.h"

void playing_std(const string &key){
    if(key == "UP"){
        lock_ui();
        if(playing_position == moving_position){
            if(cursor_pos_x > 0){
                PP_move_cursor(0);
            }
        }
        unlock_ui();
    }
    else if(key == "DOWN"){
        lock_ui();
        if(playing_position == moving_position){
            if(cursor_pos_x < Board_size - 1){
                PP_move_cursor(2);
            }
        }
        unlock_ui();
    }
    else if(key == "LEFT"){
        lock_ui();
        if(playing_position == moving_position){
            if(cursor_pos_y > 0){
                PP_move_cursor(3);
            }
        }
        unlock_ui();
    }
    else if(key == "RIGHT"){
        lock_ui();
        if(playing_position == moving_position){
            if(cursor_pos_y < Board_size - 1){
                PP_move_cursor(1);
            }
        }
        unlock_ui();
    }
    else if(key == "ENTER"){
        if(playing_position == moving_position){
            bool success = 0;
            int x, y, type;
            int remain_time[2];
            lock_ui();
            x = cursor_pos_x;
            y = cursor_pos_y;
            if(playing_board.board_data[x][y] == 0){
                type = my_piece_type;
                if(my_piece_type == 1){
                    my_piece_type = 3;
                }
                else if(my_piece_type == 3){
                    my_piece_type = 1;
                }
                else if(my_piece_type == 7){
                    my_piece_type = 9;
                }
                else if(my_piece_type == 9){
                    my_piece_type = 7;
                }
                success = 1;
                moving_position = 0;
                PP_close_timer();
                remain_time[0] = player_remain_time[0];
                remain_time[1] = player_remain_time[1];
            }
            unlock_ui();
            if(success){
                lock_writer();
                command_to_be_sent.push(
                    std::to_string(C_playing_move_finish)+" "+
                    std::to_string(x) + " " +
                    std::to_string(y) + " " +
                    std::to_string(type) + " " +
                    std::to_string(remain_time[0]) + " " +
                    std::to_string(remain_time[1]) + "\n"
                );
                signal_writer();
                unlock_writer();
            }
        }
    }
}