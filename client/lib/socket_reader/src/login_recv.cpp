#include "../socket_reader.h"

void login_with_account_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id) {
        case C_account_does_not_exist:
        break;
        case C_password_incorrect:
        break;
        case C_already_login:
        break;
        case C_login_success:
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
        break;
        case C_login_success:
        default:
        break;
    }
}