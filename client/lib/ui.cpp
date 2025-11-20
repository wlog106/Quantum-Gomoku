#include "client.h"

#define CSI "\x1b["

#define CLEAR_SCREEN   "\x1b[2J"
#define CLEAR_LINE     "\x1b[2K"
#define CURSOR_HOME    "\x1b[H"
#define CURSOR_HIDE    "\x1b[?25l"
#define CURSOR_SHOW    "\x1b[?25h"

#define MOVE(row, col)  CSI << row << ";" << col << "H"

void *ui(void *vptr){
    (void)vptr;

    int client_state = get_state();
    int ui_choose_enter;
    string ui_account_input_box;
    string ui_password_input_box;
    string ui_password_confirm_input_box;
    while(true){
        /*
        use switch case to determine what info to show
        */
        switch (client_state){
        case S_login_option:
            cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE;
            cout << "\x1b[1;38;5;226mWelcome to Quantum Gomoku!\n\x1b[0m" << flush;
            cout << "\x1b[1mPress 1 to login\nPress 2 to create new account\x1b[0m" << flush;
            break;
        case S_login_with_account:
            cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[0m" << flush;
            cout << ((ui_choose_enter == 0) ? ">" : " ");
            cout << "\x1b[1mAccount:\x1b[0m " << ui_account_input_box << '\n';
            cout << ((ui_choose_enter == 1) ? ">" : " ");
            cout << "\x1b[1mPassword:\x1b[0m ";
            for(int i = 0; i < int(ui_password_input_box.size()); i++) cout << "*";
            cout << "\nPress Esc to leave\nPress enter to confirm\nPress up/down arrow to select" << flush;
            break;
        case S_creating_account:
            cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << "\x1b[0m" << flush;
            cout << ((ui_choose_enter == 0) ? ">" : " ");
            cout << "\x1b[1mAccount:\x1b[0m " << ui_account_input_box << '\n';
            cout << ((ui_choose_enter == 1) ? ">" : " ");
            cout << "\x1b[1mPassword:\x1b[0m ";
            for(int i = 0; i < int(ui_password_input_box.size()); i++) cout << "*";
            cout << '\n';
            cout << ((ui_choose_enter == 2) ? ">" : " ");
            cout << "\x1b[1mPassword confirm:\x1b[0m ";
            for(int i = 0; i < int(ui_password_confirm_input_box.size()); i++) cout << "*";
            cout << "\nPress Esc to leave\nPress enter to confirm\nPress up/down arrow to select" << flush;
            break;
        default:
            break;
        }

        Pthread_mutex_lock(&ui_mutex);
        Pthread_cond_wait(&ui_cond, &ui_mutex);
        if(ui_end){
            Pthread_mutex_unlock(&ui_mutex);
            break;
        }
        client_state = get_state();
        /*
        use switch case to determine what info to copy
        */
        switch (client_state)
        {
        case S_login_with_account:
            ui_choose_enter = choose_enter;
            ui_account_input_box = account_input_box;
            ui_password_input_box = password_input_box;
            break;
        case S_creating_account:
            ui_choose_enter = choose_enter;
            ui_account_input_box = account_input_box;
            ui_password_input_box = password_input_box;
            ui_password_confirm_input_box = password_confirm_input_box;
            break;
        default:
            break;
        }

        Pthread_mutex_unlock(&ui_mutex);
        
    }
    return NULL;
}