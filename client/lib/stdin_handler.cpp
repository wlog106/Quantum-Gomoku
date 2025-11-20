#include "client.h"

/**
 * process esc key
 */
string get_key(){
    char ch;
    string key = "";

    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(STDIN_FILENO, &rset);
    FD_SET(std_handler_end[0], &rset);
    int maxfdp1 = max(STDIN_FILENO, std_handler_end[0]) + 1;
    Select(maxfdp1, &rset, nullptr, nullptr, nullptr);
    if(FD_ISSET(std_handler_end[0], &rset)){
        Read(std_handler_end[0], &ch, 1);
        return string("TERMINATE");
    }
    if(FD_ISSET(STDIN_FILENO, &rset)){
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
        else if(ch >= 33 && ch <= 126){
            key += ch;
            return key;
        }
    }
    return "UNKNOWN";
}

void refresh_ui(){
    Pthread_mutex_lock(&ui_mutex);
    Pthread_cond_signal(&ui_cond);
    Pthread_mutex_unlock(&ui_mutex);
}

void *stdin_handler(void *vptr){
    (void)vptr;

    int state;
    string key;
    while(true){
        key = get_key();
        if(key == "TERMINATE") {
            Pthread_mutex_lock(&ui_mutex);
            ui_end = 1;
            Pthread_cond_signal(&ui_cond);
            Pthread_mutex_unlock(&ui_mutex);
            break;
        }
        state = get_state();
        switch (state){
        case S_login_option:
            if(key == "1"){
                set_state(S_login_with_account);
                refresh_ui();
            }
            else if(key == "2"){
                set_state(S_creating_account);
                refresh_ui();
            }
            break;
        case S_login_with_account:
            if(key == "ESC"){
                set_state(S_login_option);
                Pthread_mutex_lock(&ui_mutex);
                choose_enter = 0;
                account_input_box = "";
                password_input_box = "";
                password_confirm_input_box = "";
                Pthread_cond_signal(&ui_cond);
                Pthread_mutex_unlock(&ui_mutex);
                break;
            }
            Pthread_mutex_lock(&ui_mutex);
            if(key == "UP" && choose_enter == 1){
                choose_enter = 0;
            } 
            else if(key == "DOWN" && choose_enter == 0){
                choose_enter = 1;
            }
            else if(key == "BACKSPACE"){
                if(choose_enter == 0){
                    if(!account_input_box.empty()) account_input_box.pop_back();
                }
                else if(choose_enter == 1){
                    if(!password_input_box.empty()) password_input_box.pop_back();
                }
            }
            else if(int(key.size()) == 1){
                if(choose_enter == 0 && int(account_input_box.size() < MAX_ACCOUNT_LEN)){
                    account_input_box += key;
                }
                else if(choose_enter == 1 && int(password_input_box.size() < MAX_ACCOUNT_LEN)){
                    password_input_box += key;
                }
            }
            Pthread_cond_signal(&ui_cond);
            Pthread_mutex_unlock(&ui_mutex);
            break;
        case S_creating_account:
            if(key == "ESC"){
                set_state(S_login_option);
                Pthread_mutex_lock(&ui_mutex);
                choose_enter = 0;
                account_input_box = "";
                password_input_box = "";
                password_confirm_input_box = "";
                Pthread_cond_signal(&ui_cond);
                Pthread_mutex_unlock(&ui_mutex);
                break;
            }
            Pthread_mutex_lock(&ui_mutex);
            if(key == "UP"){
                if(choose_enter == 1) choose_enter = 0;
                else if(choose_enter == 2) choose_enter = 1;
            } 
            else if(key == "DOWN"){
                if(choose_enter == 1) choose_enter = 2;
                else if(choose_enter == 0) choose_enter = 1;
            }
            else if(key == "BACKSPACE"){
                if(choose_enter == 0){
                    if(!account_input_box.empty()) account_input_box.pop_back();
                }
                else if(choose_enter == 1){
                    if(!password_input_box.empty()) password_input_box.pop_back();
                }
                else if(choose_enter == 2){
                    if(!password_confirm_input_box.empty()) password_confirm_input_box.pop_back();
                }
            }
            else if(int(key.size()) == 1){
                if(choose_enter == 0 && int(account_input_box.size() < MAX_ACCOUNT_LEN)){
                    account_input_box += key;
                }
                else if(choose_enter == 1 && int(password_input_box.size() < MAX_ACCOUNT_LEN)){
                    password_input_box += key;
                }
                else if(choose_enter == 2 && int(password_confirm_input_box.size() < MAX_ACCOUNT_LEN)){
                    password_confirm_input_box += key;
                }
            }
            Pthread_cond_signal(&ui_cond);
            Pthread_mutex_unlock(&ui_mutex);
            break;
        default:
            break;
        }
    }
    
    
    return NULL;
}