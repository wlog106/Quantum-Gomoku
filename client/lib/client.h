#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"
#include "state.h"
#include <pthread.h>

#define MAX_ACCOUNT_LEN 20

#define ALT_SCREEN_ON   "\x1b[?1049h"
#define ALT_SCREEN_OFF  "\x1b[?1049l"
#define CURSOR_SHOW     "\x1b[?25h"

/*
see extern variable in client_variable.cpp
*/
extern int sockfd;
extern pthread_mutex_t sockfd_mutex;

extern int client_state;
extern pthread_mutex_t client_state_mutex;

extern pthread_mutex_t writer_mutex;
extern pthread_cond_t writer_cond;
extern bool writer_end;
extern queue<string> command_to_be_sent;

/*
--------------------
start of ui variable
--------------------
*/
extern pthread_mutex_t ui_mutex;
extern pthread_cond_t ui_cond;
extern bool ui_end;
extern bool ui_new_info;

//login page
extern string account_input_box;
extern string password_input_box;
extern string password_confirm_input_box;
extern int login_err;//0 no error, 1 input empty, 2 password confirm does not match
extern int choose_enter;//0 enter account, 1 enter password, 2 comfirm password
/*
--------------------
end of ui variable
--------------------
*/


/*
end pipe
*/
#define CLOSE_CLIENT() write(client_end_pipe[1], "x", 1)
extern int client_end_pipe[2];
extern int std_handler_end_pipe[2];
extern int ui_end_pipe[2];
extern int socket_reader_end_pipe[2];
extern int socket_writer_end_pipe[2];

void *terminator(void *vptr);
void *stdin_handler(void *vptr);
void *ui(void *vptr);
void *socket_reader(void *vptr);
void *socket_writer(void *vptr);

void set_state(int new_state);
int  get_state();
int  get_sockfd();

int  start_connection(char* addr);

void set_terminal();
void restore_terminal();

#define lock_ui() \
    do { Pthread_mutex_lock(&ui_mutex); } while(0)

#define unlock_ui() \
    do { Pthread_mutex_unlock(&ui_mutex); } while(0)

#define signal_ui() \
    do { \
        ui_new_info = 1; \
        Pthread_cond_signal(&ui_cond); \
    } while (0)

#define wait_ui() \
    do { Pthread_cond_wait(&ui_cond, &ui_mutex); } while(0)

//writer
#define lock_writer() \
    do { Pthread_mutex_lock(&writer_mutex); } while(0)

#define unlock_writer() \
    do { Pthread_mutex_unlock(&writer_mutex); } while(0)

#define signal_writer() \
    do { Pthread_cond_signal(&writer_cond); } while (0)

#define wait_writer() \
    do { Pthread_cond_wait(&writer_cond, &writer_mutex); } while(0)