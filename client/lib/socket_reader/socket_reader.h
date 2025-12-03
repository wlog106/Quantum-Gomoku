#pragma once
#include "../client.h"

void login_with_account_recv(const string &command);
void creating_account_recv(const string &command);

//opselect
void select_option_recv(const string &command);
void select_enter_room_id_recv(const string &command);
void read_room_info(stringstream &ss);