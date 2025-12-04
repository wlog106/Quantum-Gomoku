#pragma once
#include "../../lib/universal.h"
#include "../../lib/command.h"
#include "../../lib/board.h"
#include "state.h"
#include "ui_variable.h"

#define MAX_ACCOUNT_LEN 20
#define MAX_MESSAGE_LEN 100

#define ALT_SCREEN_ON   "\x1b[?1049h"
#define ALT_SCREEN_OFF  "\x1b[?1049l"
#define CURSOR_SHOW     "\x1b[?25h"

//sockfd
extern int             server_sockfd;
extern pthread_mutex_t sockfd_mutex;
int    get_sockfd();
void   start_connection(char* addr);

//state
extern State_t         client_state;
extern pthread_mutex_t client_state_mutex;
State_t  get_state();
void     set_state(State_t new_state);


//mutex of socket writer
extern pthread_mutex_t writer_mutex;
extern pthread_cond_t writer_cond;
extern bool writer_end;
extern queue<string> command_to_be_sent;

//mutex of ui
extern pthread_mutex_t ui_mutex;
extern pthread_cond_t ui_cond;
extern bool ui_end;
extern bool ui_new_info;

//end pipe
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

//thread
void *terminator(void *vptr);
void *stdin_handler(void *vptr);
void *ui(void *vptr);
void *socket_reader(void *vptr);
void *socket_writer(void *vptr);

//other function
bool key_press_check(const string &key);
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
