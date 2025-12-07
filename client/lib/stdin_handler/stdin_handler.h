#pragma once
#include "../client.h"

//esc handler
string get_key();

//login page
void login_option_std(const string &key);
void login_with_account_std(const string &key);
void creating_account_std(const string &key);

//opselect
void select_option_std(const string &key);
void select_enter_room_id_std(const string &key);

//waiting room
void waiting_room_std(const string &key);

//playing room
void playing_std(const string &key);