#include "../stdin_handler.h"

void login_option_std(const string &key){
    if(key == "1"){
        set_state(S_login_with_account);
        lock_ui();
        signal_ui();
        unlock_ui();
    }
    else if(key == "2"){
        set_state(S_creating_account);
        lock_ui();
        signal_ui();
        unlock_ui();
    }
    return;
}

void login_with_account_std(const string &key){
    if(key == "ESC"){
        set_state(S_login_option);
        lock_ui();
        choose_enter = 0;
        account_input_box = "";
        password_input_box = "";
        password_confirm_input_box = "";
        signal_ui();
        unlock_ui();
        return;
    }
    lock_ui();
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
    signal_ui();
    unlock_ui();
    return;
}


void creating_account_std(const string &key){
    if(key == "ESC"){
        set_state(S_login_option);
        lock_ui();
        choose_enter = 0;
        account_input_box = "";
        password_input_box = "";
        password_confirm_input_box = "";
        signal_ui();
        unlock_ui();
        return;
    }
    lock_ui();
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
    signal_ui();
    unlock_ui();
    return;
}