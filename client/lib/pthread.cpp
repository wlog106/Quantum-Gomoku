#include "client.h"


pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ui_cond;

int client_state = S_login;
pthread_mutex_t client_state_mutex = PTHREAD_MUTEX_INITIALIZER;

string account_input_box;
pthread_mutex_t account_input_box_mutex = PTHREAD_MUTEX_INITIALIZER;
string password_input_box;
pthread_mutex_t password_input_box_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_state(){
    int state;
    Pthread_mutex_lock(&client_state_mutex);
    state = client_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return state;
}