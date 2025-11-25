# include "../stdin_handler.h"
#include <string>

void select_option_std(const string &key){
    if(key == "UP"){
        if(opselect_option > 0){
            lock_ui();
            opselect_option--;
            signal_ui();
            unlock_ui();
        }
    }
    else if(key == "DOWN"){
        if(opselect_option < 3){
            lock_ui();
            opselect_option++;
            signal_ui();
            unlock_ui();
        }
    }
    else if(key == "ESC"){
        set_state(S_login_option);
        lock_ui();
        reset_opselect_ui();
        signal_ui();
        unlock_ui();
        lock_writer();
        command_to_be_sent.push(std::to_string(C_client_logout) + '\n');
        signal_writer();
        unlock_writer();
    }
    else if(key == "ENTER"){

    }
    return;
}