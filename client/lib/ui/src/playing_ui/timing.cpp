#include "playing_ui.h"

struct timer_arg_t{
    int player1_time;
    int player2_time;
    bool moving_position;
};
pthread_t tid_timer;
int timer_start = 0;
int timer_runing = 0;
int timer_finish_player_remain_time[2];


void set_timer(){
    if(timer_start == 1 && timer_runing == 1)return;
    timer_arg_t *argument = new timer_arg_t{player_remain_time[0], player_remain_time[1], bool(moving_position-1)};
    timer_runing = 1;
    timer_start = 1;
    Pthread_create(&tid_timer, NULL, &time_countdown, argument);
    return;
}

bool PP_close_timer(){ //close timer and set the remain time
    if(timer_start == 0) return 0;
    bool flag = timer_runing;
    timer_start = 0;
    timer_runing = 0;
    unlock_ui();
    Pthread_join( tid_timer, NULL);
    lock_ui();
    player_remain_time[0] = timer_finish_player_remain_time[0];
    player_remain_time[1] = timer_finish_player_remain_time[1];
    return flag;
}

void print_time(int remain_time){
    int second = (remain_time / 10) % 60;
    int minute = remain_time / 600;
    if(minute > 0){
        if(minute < 10) cout << ' ';
        cout << minute << ':';
        if(second < 10) cout << '0';
        cout << second;
    }
    else{
        cout << ' ';
        if(second < 10) cout << ' ';
        cout << second << '.' << remain_time % 10;
    }
}

void *time_countdown(void *vptr){
    timer_arg_t* arg = (timer_arg_t*)vptr;
    auto [player1_time, player2_time, target] = *arg;
    delete arg;

    if(target){ //player 2 is moving
        cout << MOVE(((int)Timer_pos_x + 0), Timer_pos_y);
        print_time(player1_time);
    }
    else{
        cout << MOVE(((int)Timer_pos_x + 1), Timer_pos_y);
        print_time(player2_time);
    }
    cout << flush;

    timespec next;
    clock_gettime(CLOCK_MONOTONIC, &next);

    int &counter = (target ? player2_time : player1_time);
    while (counter >= 0 && timer_runing) {
        lock_ui();
        cout << MOVE(((int)Timer_pos_x + target), Timer_pos_y);
        print_time(counter);
        cout << flush;
        counter--;
        unlock_ui();

        next.tv_nsec += 100 * 1000 * 1000;
        if (next.tv_nsec >= 1000000000L) {
            next.tv_sec += 1;
            next.tv_nsec -= 1000000000L;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, nullptr);
    }
    timer_finish_player_remain_time[0] = player1_time;
    timer_finish_player_remain_time[1] = player2_time;
    lock_ui();
    if(timer_runing && moving_position == playing_position){
        bool success = 0;
        int x, y, type;
        x = cursor_pos_x;
        y = cursor_pos_y;
        moving_position = 0;
        PP_erase_cursor();
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
        }
        unlock_ui();
        lock_writer();
        if(success){
            command_to_be_sent.push(
                std::to_string(C_playing_move_finish)+" "+
                std::to_string(x) + " " +
                std::to_string(y) + " " +
                std::to_string(type) + " " +
                std::to_string(timer_finish_player_remain_time[0]) + " " +
                std::to_string(timer_finish_player_remain_time[1]) + "\n"
            );
        }
        else{
            command_to_be_sent.push(
                std::to_string(C_run_out_of_time) + "\n"
            );
            
        }
        signal_writer();
        unlock_writer();
    }
    else{
        unlock_ui();
    }
    timer_runing = 0;
    return NULL;
}
