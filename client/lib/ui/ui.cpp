#include "ui.h"
#include <sys/select.h>
#include <unistd.h>



void *ui(void *vptr){
    (void)vptr;

    int client_state;

    while(true){
        fd_set rset, wset;
        FD_ZERO(&rset);
        FD_SET(ui_end_pipe[0], &rset);
        FD_ZERO(&wset);
        FD_SET(STDOUT_FILENO, &wset);
        int maxfdp1 = max(STDOUT_FILENO, ui_end_pipe[0]) + 1;
        Select(maxfdp1, &rset, &wset, nullptr, nullptr);
        if(FD_ISSET(ui_end_pipe[0], &rset)){
            char ch;
            Read(ui_end_pipe[0], &ch, 1);
            break;
        }
        
        if(FD_ISSET(STDOUT_FILENO, &wset)){
            lock_ui();

            if(!ui_new_info){
                wait_ui();
            }
            ui_new_info = 0;
            if(ui_end){
                unlock_ui();
                break;
            }

            client_state = get_state();
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
                cout << ((choose_enter == 0) ? ">" : " ");
                cout << "\x1b[1mAccount:\x1b[0m " << account_input_box << '\n';
                cout << ((choose_enter == 1) ? ">" : " ");
                cout << "\x1b[1mPassword:\x1b[0m ";
                for(int i = 0; i < int(password_input_box.size()); i++) cout << "*";
                cout << "\nPress Esc to leave\nPress enter to confirm\nPress up/down arrow to select" << flush;
                break;
            case S_creating_account:
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
                cout << "\nPress Esc to leave\nPress enter to confirm\nPress up/down arrow to select" << flush;
                break;
            default:
                break;
            }
            unlock_ui();
        }
    }
    return NULL;
}