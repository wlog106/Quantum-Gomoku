#include "../stdin_handler.h"
#include "sha256.h"

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
        reset_login_ui();
        signal_ui();
        unlock_ui();
    }
    else if(key == "UP" && choose_enter == 1){
        lock_ui();
        choose_enter = 0;
        signal_ui();
        unlock_ui();
    } 
    else if(key == "DOWN" && choose_enter == 0){
        lock_ui();
        choose_enter = 1;
        signal_ui();
        unlock_ui();
    }
    else if(key == "BACKSPACE"){
        lock_ui();
        if(choose_enter == 0){
            if(!account_input_box.empty()) account_input_box.pop_back();
        }
        else if(choose_enter == 1){
            if(!password_input_box.empty()) password_input_box.pop_back();
        }
        signal_ui();
        unlock_ui();
    }
    else if(key == "ENTER"){
        if(!key_press_check(key)) return;
        lock_ui();
        string account = account_input_box;
        string password = password_input_box;
        unlock_ui();
        if(account.empty() || password.empty()){
            lock_ui();
            login_err = LE_input_empty;
            signal_ui();
            unlock_ui();
        }
        else{
            char hex[SHA256_HEX_SIZE];
            sha256_hex(password.c_str(), password.size(), hex);
            string command = std::to_string(C_login_to_server) + " " + account + " " + hex + "\n";
            lock_ui();
            login_err = LE_waiting;
            password_input_box = "";
            signal_ui();
            unlock_ui();
            lock_writer();
            command_to_be_sent.push(command);
            signal_writer();
            unlock_writer();
        }
    }
    else if(int(key.size()) == 1){
        lock_ui();
        if(choose_enter == 0 && int(account_input_box.size()) < MAX_ACCOUNT_LEN){
            account_input_box += key;
        }
        else if(choose_enter == 1 && int(password_input_box.size()) < MAX_ACCOUNT_LEN){
            password_input_box += key;
        }
        signal_ui();
        unlock_ui();
    }
    return;
}


void creating_account_std(const string &key){
    if(key == "ESC"){
        set_state(S_login_option);
        lock_ui();
        reset_login_ui();
        signal_ui();
        unlock_ui();
    }
    else if(key == "UP"){
        lock_ui();
        if(choose_enter == 1) choose_enter = 0;
        else if(choose_enter == 2) choose_enter = 1;
        signal_ui();
        unlock_ui();
    } 
    else if(key == "DOWN"){
        lock_ui();
        if(choose_enter == 1) choose_enter = 2;
        else if(choose_enter == 0) choose_enter = 1;
        signal_ui();
        unlock_ui();
    }
    else if(key == "BACKSPACE"){
        lock_ui();
        if(choose_enter == 0){
            if(!account_input_box.empty()) account_input_box.pop_back();
        }
        else if(choose_enter == 1){
            if(!password_input_box.empty()) password_input_box.pop_back();
        }
        else if(choose_enter == 2){
            if(!password_confirm_input_box.empty()) password_confirm_input_box.pop_back();
        }
        signal_ui();
        unlock_ui();
    }
    else if(key == "ENTER"){
        if(!key_press_check(key)) return;
        lock_ui();
        string account = account_input_box;
        string password = password_input_box;
        string password_confirm = password_confirm_input_box;
        unlock_ui();
        if(account.empty() || password.empty() || password_confirm.empty()){
            lock_ui();
            login_err = LE_input_empty;
            signal_ui();
            unlock_ui();
        }
        else if(password != password_confirm){
            lock_ui();
            login_err = LE_password_confirm_does_not_match;
            signal_ui();
            unlock_ui();
        }
        else{
            char hex[SHA256_HEX_SIZE];
            sha256_hex(password.c_str(), password.size(), hex);
            string command = std::to_string(C_create_new_account) + " " + account + " " + hex + "\n";
            lock_ui();
            password_input_box = "";
            password_confirm_input_box = "";
            login_err = LE_waiting;
            signal_ui();
            unlock_ui();
            lock_writer();
            command_to_be_sent.push(command);
            signal_writer();
            unlock_writer();
        }
    }
    else if(int(key.size()) == 1){
        lock_ui();
        if(choose_enter == 0 && int(account_input_box.size() < MAX_ACCOUNT_LEN)){
            account_input_box += key;
        }
        else if(choose_enter == 1 && int(password_input_box.size() < MAX_ACCOUNT_LEN)){
            password_input_box += key;
        }
        else if(choose_enter == 2 && int(password_confirm_input_box.size() < MAX_ACCOUNT_LEN)){
            password_confirm_input_box += key;
        }
        signal_ui();
        unlock_ui();
    }
    return;
}