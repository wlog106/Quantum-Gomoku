#pragma once
#include "../client.h"

//login
void login_with_account_recv(const string &command);
void creating_account_recv(const string &command);

//opselect
void select_option_recv(const string &command);
void select_enter_room_id_recv(const string &command);
void read_room_info(stringstream &ss);

//waiting room
void waiting_room_recv(const string &command);

//playing room
void playing_recv(const string &command);
void read_playing_info(stringstream &ss);
void read_segment_info(stringstream &ss);