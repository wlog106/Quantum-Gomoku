#include "client.h"
#include <pthread.h>
#include <sys/select.h>


int client_state;
int sockfd;
pthread_mutex_t sockfd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_state_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t writer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t writer_cond;
bool writer_end = 0;
queue<string> command_to_be_sent;

/*
--------------------
start of ui variable
--------------------
*/
pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER; //lock everything about ui
pthread_cond_t ui_cond;
bool ui_end = 0;
bool ui_new_info = 1;

//login page
string account_input_box;
string password_input_box;
string password_confirm_input_box;
int login_err = 0;
int choose_enter = 0;
/*
--------------------
end of ui variable
--------------------
*/

int client_end_pipe[2];
int std_handler_end_pipe[2];
int ui_end_pipe[2];
int socket_reader_end_pipe[2];
int socket_writer_end_pipe[2];

void *terminator(void *vptr){
    pipe(ui_end_pipe);
    pipe(std_handler_end_pipe);
    pipe(socket_reader_end_pipe);
    pipe(socket_writer_end_pipe);
    
    fd_set set;
    FD_ZERO(&set);
    FD_SET(client_end_pipe[0], &set);
    Select(client_end_pipe[0]+1, &set, nullptr, nullptr, nullptr);
    char ch;
    Read(client_end_pipe[0], &ch, 1);
    write(std_handler_end_pipe[1], "x", 1);
    write(ui_end_pipe[1], "x", 1); //order does matter
    write(socket_reader_end_pipe[1], "x", 1);
    write(socket_writer_end_pipe[1], "x", 1); //order does matter
    return NULL;
}

int get_state(){
    int state;
    Pthread_mutex_lock(&client_state_mutex);
    state = client_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return state;
}

int get_sockfd(){
    int sockfd;
    Pthread_mutex_lock(&sockfd_mutex);
    sockfd = client_state;
    Pthread_mutex_unlock(&sockfd_mutex);
    return sockfd;
}

void set_state(int new_state){
    Pthread_mutex_lock(&client_state_mutex);
    client_state = new_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return;
}

int start_connection(char* addr){
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TEST_PORT);
    Inet_pton(AF_INET, addr, &servaddr.sin_addr);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    return sockfd;
}

//terminal
struct termios oldt, newt;

void set_terminal(){
    tcgetattr(STDIN_FILENO, &oldt); 
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag&= ~( ECHO | ICANON | ECHOE | ECHOK |
    ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cout << ALT_SCREEN_ON << flush;

}

void restore_terminal(){
    cout << ALT_SCREEN_OFF << CURSOR_SHOW << flush;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}