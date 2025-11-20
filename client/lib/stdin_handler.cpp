#include "client.h"

struct terminalsetting{
    struct termios oldt, newt;
    terminalsetting(){
        tcgetattr(STDIN_FILENO, &oldt); 
        memcpy(&newt, &oldt, sizeof(newt));
        newt.c_lflag&= ~( ECHO | ICANON | ECHOE | ECHOK |
        ECHONL | ECHOPRT | ECHOKE | ICRNL);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    ~terminalsetting(){
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
};

/**
 * process esc key
 */
string get_key(){
    int ch;
    string key;
    ch = getchar();
    if (ch == 27) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 20000;   // 20 ms timeout

        int r = select(STDIN_FILENO + 1, &set, NULL, NULL, &tv);
        if (r > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            int c2 = getchar();
            if (c2 == '[') {
                int c3 = getchar();
                if (c3 == 'A') key = "UP";
                else if (c3 == 'B') key = "DOWN";
                else if (c3 == 'C') key = "RIGHT";
                else if (c3 == 'D') key = "LEFT";
                else key = "UNKNOWN";
            } else {
                key = "ESC"; //ignore that key;
            }
        } else {
            key = "ESC";
        }
    } else {
        key = ch;
    }
    return key;
}

void *stdin_handler(void *vptr){
    (void)vptr;
    
    terminalsetting ter_set;

    int state;
    string key;
    while(true){
        key = get_key();
        state = get_state();
        switch (state){
        case S_login_option:
            break;
        
        default:
            break;
        }
    }
    
    
    return NULL;
}