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
                login_option_ui();
                break;
            case S_login_with_account:
                login_with_account_ui();
                break;
            case S_creating_account:
                creating_account_ui();
                break;
            default:
                break;
            }
            unlock_ui();
        }
    }
    return NULL;
}