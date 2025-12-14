# include "../stdin_handler.h"

void playing_std(const string &key){
    if(key == "UP"){
        lock_ui();
        if(playing_position == moving_position && playing_page_type == 1 && !game_over){
            if(cursor_pos_x > 0){
                PP_move_cursor(0);
            }
        }
        unlock_ui();
    }
    else if(key == "DOWN"){
        lock_ui();
        if(playing_position == moving_position && playing_page_type == 1 && !game_over){
            if(cursor_pos_x < Board_size - 1){
                PP_move_cursor(2);
            }
        }
        unlock_ui();
    }
    else if(key == "LEFT"){
        lock_ui();
        if(playing_position == moving_position && playing_page_type == 1 && !game_over){
            if(cursor_pos_y > 0){
                PP_move_cursor(3);
            }
        }
        unlock_ui();
    }
    else if(key == "RIGHT"){
        lock_ui();
        if(playing_position == moving_position && playing_page_type == 1 && !game_over){
            if(cursor_pos_y < Board_size - 1){
                PP_move_cursor(1);
            }
        }
        unlock_ui();
    }
    else if(key == "ENTER"){
        lock_ui();
        if(game_over){
            set_state(S_select_option);
            reset_playing_page();
            signal_ui();
            unlock_ui();
            return;
        }
        if(playing_position == moving_position && playing_page_type == 1){
            bool success = 0;
            int x, y, type;
            int remain_time[2];
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
                success = PP_close_timer();
                if(success) {
                    moving_position = 0;
                    PP_erase_cursor();
                }
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
        else{
            unlock_ui();
        }
    }
    else if(key == "O" || key == "o"){
        bool observe = 0;
        lock_ui();
        if(playing_page_type == 1 && has_observe == 0 && !game_over && observing_chance > 0){
            has_observe = 1;
            observe = 1;
            observing_chance--;
            PP_refresh_observe_chance_info();
        }
        unlock_ui();
        if(observe){
            lock_writer();
            command_to_be_sent.push(
                    std::to_string(C_want_to_observe) + "\n"
                );
            signal_writer();
            unlock_writer();
        }
    }
    else if(key == "L" || key == "l"){
        if(game_over){
            set_state(S_select_option);
            lock_ui();
            PP_close_timer();
            reset_playing_page();
            signal_ui();
            unlock_ui();
            return;
        }
        if(playing_page_type == 1 && playing_position != 1 && playing_position != 2){
            lock_writer();
            command_to_be_sent.push(
                    std::to_string(C_leave_playing_room) + "\n"
                );
            signal_writer();
            unlock_writer();
            set_state(S_select_option);
            lock_ui();
            PP_close_timer();
            reset_playing_page();
            signal_ui();
            unlock_ui();
        }
    }
}