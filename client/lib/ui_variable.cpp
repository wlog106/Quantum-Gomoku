#include "ui_variable.h"
/*
This file is for initialization of ui variable.
Also reset part of ui variable
*/

//login page
//-------------------------------------------------------------------------------
string account_input_box;
string password_input_box;
string password_confirm_input_box;
login_error_t login_err = LE_no_error;
int choose_enter = 0;
void reset_login_ui(){
    account_input_box = "";
    password_input_box = "";
    password_confirm_input_box = "";
    login_err = LE_no_error;
    choose_enter = 0;
    return;
}

//select page
//-------------------------------------------------------------------------------
int opselect_option = 0;
opselect_reply_t opselect_reply = OSR_no_error;
void reset_opselect_ui(){
    opselect_option = 0;
    opselect_reply = OSR_no_error;
    return;
}

//select room id
string room_id_input_box;
void reset_opselect_room_id(){
    room_id_input_box = "";
    opselect_reply = OSR_no_error;
}


//waiting room page
//-------------------------------------------------------------------------------
string waiting_room_id = "";
bool waiting_user_existance[5] = {false, false, false, false, false};
string waiting_username[5] = {"", "", "", "", ""};
bool waiting_is_ready[2] = {false, false};
int waiting_room_entering = 0;
deque<string> waiting_room_history_message;
string waiting_room_input_box = "";

void reset_waiting_room(){
    waiting_room_id = "";
    for(int i = 0; i < 5; i++) waiting_user_existance[i] = false;
    for(int i = 0; i < 5; i++) waiting_username[i] = "";
    for(int i = 0; i < 2; i++) waiting_is_ready[i] = false;
    waiting_room_entering = 0;
    waiting_room_history_message.clear();
    waiting_room_input_box = "";
    return;
}