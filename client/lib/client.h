#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"

#define S_end 0
#define S_login_option 1
#define S_op_select 2
#define S_orphan 3
#define S_waiting 4
#define S_my_turn 5
#define S_oppo_turn 6
#define S_observer 7
#define S_observing 8
#define S_showresult 9
//max-id = 10


/*
see extern variable in pthread.cpp
*/
extern int sockfd;
extern pthread_mutex_t sockfd_mutex;

extern int client_state;
extern pthread_mutex_t client_state_mutex;

extern pthread_mutex_t ui_mutex;
extern pthread_cond_t ui_cond;
extern bool ui_end;
extern string account_input_box;
extern string password_input_box;

void *stdin_handler(void *vptr);
void *ui(void *vptr);
void *socket_handler(void *vptr);

void change_state(int new_state);
int get_state();

int start_connection(char* addr);

void set_terminal();
void restore_terminal();

int Login(int sockfd);
