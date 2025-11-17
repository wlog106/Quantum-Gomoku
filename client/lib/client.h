#pragma once
#include "../../lib/universal.h"

#define S_end 0
#define S_login 1
#define S_op_select 2
#define S_orphan 3
#define S_waiting 4
#define S_my_turn 5
#define S_oppo_turn 6
#define S_observer 7
#define S_observing 8
#define S_showresult 9

void Login(int sockfd);