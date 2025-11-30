#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"
#include "../../lib/board.h"
#include "state.h"
#include <pthread.h>

#define MAX_ACCOUNT_LEN 20
#define ROOM_ID_LEN 6

#define ALT_SCREEN_ON   "\x1b[?1049h"
#define ALT_SCREEN_OFF  "\x1b[?1049l"
#define CURSOR_SHOW     "\x1b[?25h"

/*
see extern variable in client_variable.cpp
*/
extern int sockfd;
extern pthread_mutex_t sockfd_mutex;

extern State_t client_state;
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
extern int opselect_option;
typedef enum{
    OSR_no_error,
    OSR_waiting,
    OSR_room_id_len_error
}opselect_reply_t;
extern opselect_reply_t opselect_reply;
void reset_opselect_ui();

//select room id page
extern string room_id_input_box;
void reset_opselect_room_id();

/*
--------------------
end of ui variable
--------------------
*/


/*
end pipe
*/
#define CLOSE_CLIENT(x) \
    do { \
        char _tmp = (x); \
        write(client_end_pipe[1], &_tmp, 1); \
    } while (0)
extern char client_end_code;
extern int  client_end_pipe[2];
extern int  std_handler_end_pipe[2];
extern int  ui_end_pipe[2];
extern int  socket_reader_end_pipe[2];
extern int  socket_writer_end_pipe[2];

void *terminator(void *vptr);
void *stdin_handler(void *vptr);
void *ui(void *vptr);
void *socket_reader(void *vptr);
void *socket_writer(void *vptr);

void     set_state(State_t new_state);
State_t  get_state();
int      get_sockfd();

bool enter_press_check();

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
