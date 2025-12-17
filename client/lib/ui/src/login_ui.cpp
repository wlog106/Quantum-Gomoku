#include "../ui.h"


void login_option_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE;
    int w = 40, h = 9;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            if(i == 0 || j == 0 || i == h-1 || j == w-1){
                if((i+j) & 1){
                    cout << "\x1b[48;5;255m   \x1b[0m";
                }
                else{
                    cout << "\x1b[48;5;0m   \x1b[0m";
                }
            }
            else{
                cout << "   ";
            }
        }
        cout << endl;
    }
    int title_pos_x = 2, title_pos_y = 4;
    string title[6] = {
"  ____                            _                             _____                               _            ",
" / __ \\                          | |                           / ____|                             | |           ",
"| |  | |  _   _    __ _   _ __   | |_   _   _   _ __ ___      | |  __    ___    _ __ ___     ___   | | __  _   _ ",
"| |  | | | | | |  / _` | | '_ \\  | __| | | | | | '_ ` _ \\     | | |_ |  / _ \\  | '_ ` _ \\   / _ \\  | |/ / | | | |",
"| |__| | | |_| | | (_| | | | | | | |_  | |_| | | | | | | |    | |__| | | (_) | | | | | | | | (_) | |   <  | |_| |",
" \\___\\_\\  \\__,_|  \\__,_| |_| |_|  \\__|  \\__,_| |_| |_| |_|     \\_____|  \\___/  |_| |_| |_|  \\___/  |_|\\_\\  \\__,_|"};

    for(int i = 0; i < 6; i++){
        cout << MOVE(title_pos_x + i, title_pos_y) << title[i];
    }

    cout << MOVE(12, 19) << "\x1b[1;38;5;82mPress 1 to login\n";
    cout << MOVE(12, 84) << "\x1b[1;38;5;82mPress 2 to create new account\x1b[0m" << flush;
    cout << MOVE(14, 47) << "\x1b[1;38;5;196mPress Esc to close the game\x1b[0m" << flush;
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

