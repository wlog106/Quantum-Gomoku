#include "client.h"


int client_state;
int sockfd;
pthread_mutex_t sockfd_mutex;
pthread_mutex_t client_state_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER; //lock everything about ui
pthread_cond_t ui_cond;
bool ui_end = 0;
string account_input_box;
string password_input_box;

int get_state(){
    int state;
    Pthread_mutex_lock(&client_state_mutex);
    state = client_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return state;
}

void change_state(int new_state){
    Pthread_mutex_lock(&client_state_mutex);
    client_state = new_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return;
}