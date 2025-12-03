#include "../ui.h"


void login_option_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE;
    cout << "\x1b[1;38;5;226mWelcome to Quantum Gomoku!\n\x1b[0m" << flush;
    cout << "\x1b[1mPress 1 to login\nPress 2 to create new account\x1b[0m" << flush;
    return;
}

void login_with_account_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[0m" << flush;
    cout << ((choose_enter == 0) ? ">" : " ");
    cout << "\x1b[1mAccount:\x1b[0m " << account_input_box << '\n';
    cout << ((choose_enter == 1) ? ">" : " ");
    cout << "\x1b[1mPassword:\x1b[0m ";
    for(int i = 0; i < int(password_input_box.size()); i++) cout << "*";
    cout << "\n" << flush;
    switch (login_err) {
        case LE_input_empty:
            cout << "\x1b[1;31mInput can not be empty!\n";
        break;
        case LE_account_does_not_exist:
            cout << "\x1b[1;31mAccount does not exist!\n";
        break;
        case LE_password_incorrect:
            cout << "\x1b[1;31mPassword is incorrect!\n";
        break;
        case LE_already_login:
            cout << "\x1b[1;31mThis account already login!\n";
        break;
        case LE_waiting:
            cout << "\x1b[1mWaiting...\n";
        break;
        default:
        break;
    }
    cout << "\x1b[0m────────────────────────────────────────\n";
    cout << "Press Esc to leave\nPress Enter to login\nPress Up/Down arrow to select" << flush;
    return;
}

void creating_account_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[0m" << flush;
    cout << ((choose_enter == 0) ? ">" : " ");
    cout << "\x1b[1mAccount:\x1b[0m " << account_input_box << '\n';
    cout << ((choose_enter == 1) ? ">" : " ");
    cout << "\x1b[1mPassword:\x1b[0m ";
    for(int i = 0; i < int(password_input_box.size()); i++) cout << "*";
    cout << '\n';
    cout << ((choose_enter == 2) ? ">" : " ");
    cout << "\x1b[1mPassword confirm:\x1b[0m ";
    for(int i = 0; i < int(password_confirm_input_box.size()); i++) cout << "*";
    cout << "\n" << flush;
    switch (login_err) {
        case LE_input_empty:
            cout << "\x1b[1;31mInput can not be empty!\n";
        break;
        case LE_password_confirm_does_not_match:
            cout << "\x1b[1;31mPassword confirm are not the same!\n";
        break;
        case LE_account_already_exist:
            cout << "\x1b[1;31mAccount already exist!\n";
        break;
        case LE_waiting:
            cout << "\x1b[1mWaiting...\n";
        break;
        default:
        break;
    }
    cout << "\x1b[0m────────────────────────────────────────\n";
    cout << "Press Esc to leave\nPress Enter to create account and login\nPress Up/Down arrow to select" << flush;
    return;
}

