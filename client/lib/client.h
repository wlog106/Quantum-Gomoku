#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"

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
//max-id = 10

extern pthread_mutex_t ui_mutex;
extern pthread_cond_t ui_cond;

extern int client_state;
extern pthread_mutex_t client_state_mutex;

extern string account_input_box;
extern pthread_mutex_t account_input_box_mutex;
extern string password_input_box;
extern pthread_mutex_t password_input_box_mutex;

int get_state();

int Login(int sockfd);
void client_nonblocking_io(int sockfd);