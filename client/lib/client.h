#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"

#define S_end 0
#define S_login_option 1
#define S_login_with_account 2
#define S_creating_account 3

#define MAX_ACCOUNT_LEN 20


/*
see extern variable in client_variable.cpp
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
extern string password_confirm_input_box;
extern int choose_enter;//0 enter account, 1 enter password, 2 comfirm password

/*
end pipe
*/
extern int std_handler_end[2];

void *stdin_handler(void *vptr);
void *ui(void *vptr);
void *socket_handler(void *vptr);

void set_state(int new_state);
int  get_state();

int  start_connection(char* addr);

void set_terminal();
void restore_terminal();

int  Login(int sockfd);
