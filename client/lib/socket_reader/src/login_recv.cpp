#include "../socket_reader.h"

void login_with_account_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id) {
        case C_account_does_not_exist:
            lock_ui();
            account_input_box = "";
            login_err = LE_account_does_not_exist;
            signal_ui();
            unlock_ui();
        break;
        case C_password_incorrect:
            lock_ui();
            login_err = LE_password_incorrect;
            signal_ui();
            unlock_ui();
        break;
        case C_already_login:
            lock_ui();
            account_input_box = "";
            login_err = LE_already_login;
            signal_ui();
            unlock_ui();
        break;
        case C_login_success:
            set_state(S_select_option);
            lock_ui();
            reset_login_ui();
            signal_ui();
            unlock_ui();
        default:
        break;
    }
}

void creating_account_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id) {
        case C_account_already_exist:
            lock_ui();
            account_input_box = "";
            login_err = LE_account_already_exist;
            signal_ui();
            unlock_ui();
        break;
        case C_login_success:
            set_state(S_select_option);
            lock_ui();
            reset_login_ui();
            signal_ui();
            unlock_ui();
        default:
        break;
    }
}