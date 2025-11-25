#include "stdin_handler.h"


void *stdin_handler(void *vptr){
    (void)vptr;

    State_t client_state;
    string key;
    char ch;
    fd_set rset;
    int maxfdp1 = max(STDIN_FILENO, std_handler_end_pipe[0]) + 1;
    int std_has_end = 0;//end if read ctrl-d
    
    while(true){
        FD_ZERO(&rset);
        if(!std_has_end) FD_SET(STDIN_FILENO, &rset);
        FD_SET(std_handler_end_pipe[0], &rset);
        Select(maxfdp1, &rset, nullptr, nullptr, nullptr);
        if(FD_ISSET(std_handler_end_pipe[0], &rset)){
            Read(std_handler_end_pipe[0], &ch, 1);
            lock_ui();
            ui_end = 1;
            signal_ui();
            unlock_ui();
            break;
        }
        if(FD_ISSET(STDIN_FILENO, &rset)){
            key = get_key();
            if(key == "EOF"){
                std_has_end = 1;
                CLOSE_CLIENT('d');
                continue;
            }
            client_state = get_state();
            switch (client_state){
            case S_login_option:
                login_option_std(key);
                break;
            case S_login_with_account:
                login_with_account_std(key);
                break;
            case S_creating_account:
                creating_account_std(key);
                break;
            default:
                break;
            }
        }
    }
    return NULL;
}


string get_key(){
    char ch;
    string key;
    Read(STDIN_FILENO, &ch, 1);
    if (ch == 27) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 20;   // 20 ms timeout

        int r = select(STDIN_FILENO + 1, &set, NULL, NULL, &tv);
        if (r > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            int c2 = getchar();
            if (c2 == '['){
                int c3 = getchar();
                if (c3 == 'A') key = "UP";
                else if (c3 == 'B') key = "DOWN";
                else if (c3 == 'C') key = "RIGHT";
                else if (c3 == 'D') key = "LEFT";
                else key = "UNKNOWN";
            }
            else{
                key = "ESC"; //ignore that key;
            }
        }
        else{
            key = "ESC";
        }
        return key;
    }
    else if(ch == 127){
        return "BACKSPACE";
    }
    else if(ch == 10){
        return "ENTER";
    }
    else if(ch == 4){
        return "EOF";
    }
    else if(ch >= 33 && ch <= 126){
        key += ch;
        return key;
    }
    return "UNKNOWN";
}