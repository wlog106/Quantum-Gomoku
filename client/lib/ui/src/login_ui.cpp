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
    cout << "\nPress Esc to leave\nPress Enter to confirm\nPress Up/Down arrow to select" << flush;
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
    cout << "\nPress Esc to leave\nPress Enter to confirm\nPress Up/Down arrow to select" << flush;
    return;
}

