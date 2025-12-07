#pragma once
#include "../../ui.h"
#define Board_pos_x 4
#define Board_pos_y 5
#define vertical_bar 1
#define horizontal_bar 4
#define Timer_pos_x 30
#define Timer_pos_y 61

#define User_info_pos_x 30
#define User_info_pos_y 5



//board
void clear_board_ui();
void draw_cursor(int x, int y);
void erase_cursor(int x, int y);
void draw_piece(int x, int y, int piece_type);

//timing
void set_timer();
//void close_timer(); define in ui.h
void *time_countdown(void *vptr);
void print_time(int remain_time);

//user
void show_all_user();
void user_change(int pos);