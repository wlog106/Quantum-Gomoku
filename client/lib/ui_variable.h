#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"
#include "../../lib/board.h"
/*
This file is for definition of ui variable.
*/


//login page
//-------------------------------------------------------------------------------
extern string account_input_box;
extern string password_input_box;
extern string password_confirm_input_box;
typedef enum{
    LE_no_error,
    LE_input_empty,
    LE_password_confirm_does_not_match,
    LE_account_does_not_exist,
    LE_password_incorrect,
    LE_already_login,
    LE_account_already_exist,
    LE_waiting
}login_error_t;
extern login_error_t login_err;
extern int choose_enter;//0 enter account, 1 enter password, 2 comfirm password
void reset_login_ui();

//select page
//-------------------------------------------------------------------------------
extern int opselect_option;
typedef enum{
    OSR_no_error,
    OSR_waiting,
    OSR_room_id_len_error,
    OSR_too_much_room,
    OSR_room_id_dne,
    OSR_room_already_full,
    OSR_no_empty_waiting_room,//for pair randomly
    OSR_no_current_playing_room //for observe randomly
}opselect_reply_t;
extern opselect_reply_t opselect_reply;
void reset_opselect_ui();

//select room id page
extern string room_id_input_box;
void reset_opselect_room_id();


//waiting room page
//-------------------------------------------------------------------------------
extern string waiting_room_id;
extern bool waiting_user_existance[5];
extern string waiting_username[5];
extern bool waiting_is_ready[2];
extern int waiting_room_entering;
extern deque<string> waiting_room_history_message;
extern string waiting_room_input_box;

void reset_waiting_room();