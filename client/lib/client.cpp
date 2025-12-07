#include "client.h"

//sockfd
int             server_sockfd;
pthread_mutex_t sockfd_mutex = PTHREAD_MUTEX_INITIALIZER;
int get_sockfd(){
    int sockfd;
    Pthread_mutex_lock(&sockfd_mutex);
    sockfd = server_sockfd;
    Pthread_mutex_unlock(&sockfd_mutex);
    return sockfd;
}
void start_connection(char* addr){
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TEST_PORT);
    Inet_pton(AF_INET, addr, &servaddr.sin_addr);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    Pthread_mutex_lock(&sockfd_mutex);
    server_sockfd = sockfd;
    Pthread_mutex_unlock(&sockfd_mutex);
    return;
}

//state
State_t         client_state;
pthread_mutex_t client_state_mutex = PTHREAD_MUTEX_INITIALIZER;
State_t get_state(){
    State_t state;
    Pthread_mutex_lock(&client_state_mutex);
    state = client_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return state;
}
void set_state(State_t new_state){
    Pthread_mutex_lock(&client_state_mutex);
    client_state = new_state;
    Pthread_mutex_unlock(&client_state_mutex);
    return;
}

//mutex of socket writer
pthread_mutex_t writer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t writer_cond;
bool writer_end = 0;
queue<string> command_to_be_sent;

//mutex of ui
pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ui_cond;
bool ui_end = 0;
bool ui_new_info = 1;

//end pipe
char client_end_code;
int client_end_pipe[2];
int std_handler_end_pipe[2];
int ui_end_pipe[2];
int socket_reader_end_pipe[2];
int socket_writer_end_pipe[2];

void *terminator(void *vptr){
    (void)vptr;

    Pipe(ui_end_pipe);
    Pipe(std_handler_end_pipe);
    Pipe(socket_reader_end_pipe);
    Pipe(socket_writer_end_pipe);
    
    fd_set set;
    FD_ZERO(&set);
    FD_SET(client_end_pipe[0], &set);
    Select(client_end_pipe[0]+1, &set, nullptr, nullptr, nullptr);
    Read(client_end_pipe[0], &client_end_code, 1);
    Write(std_handler_end_pipe[1], "x", 1);
    Write(ui_end_pipe[1], "x", 1); //order does matter
    Write(socket_reader_end_pipe[1], "x", 1);
    Write(socket_writer_end_pipe[1], "x", 1); //order does matter
    return NULL;
}

//enter spanning
long long now_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000;
}

map<string, long long> key_last_press;
const int KEY_COOLDOWN = 300; // 300ms

bool key_press_check(const string &key) {
    long long t = now_ms();
    if (t - key_last_press[key] >= KEY_COOLDOWN) {
        key_last_press[key] = t;
        return true;
    }
    return false;
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