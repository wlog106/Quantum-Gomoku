#include "ui.h"


void *ui(void *vptr){
    (void)vptr;

    State_t client_state;

    while(true){
        fd_set rset, wset;
        FD_ZERO(&rset);
        FD_SET(ui_end_pipe[0], &rset);
        FD_ZERO(&wset);
        if(!ui_end) FD_SET(STDOUT_FILENO, &wset);
        int maxfdp1 = max(STDOUT_FILENO, ui_end_pipe[0]) + 1;
        Select(maxfdp1, &rset, &wset, nullptr, nullptr);
        if(FD_ISSET(ui_end_pipe[0], &rset)){
            char ch;
            Read(ui_end_pipe[0], &ch, 1);
            break;
        }
        
        if(FD_ISSET(STDOUT_FILENO, &wset)){
            lock_ui();

            if(!ui_new_info && !ui_end){
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
            case S_select_option:
                select_option_ui();
                break;
            case S_select_enter_room_id:
                select_enter_room_id_ui();
                break;
            case S_waiting_room:
                waiting_room_ui();
                break;
            case S_debug:
                debug_ui();
                break;
            default:
                break;
            }
            unlock_ui();
        }
    }
    PP_close_timer();
    return NULL;
}

void debug_ui(){
    set_state(S_playing);
    playing_room_id = "KKKKKK";
    playing_user_existance[0] = 1;
    playing_username[0] = "01234567890123456789";
    playing_position = 1;
    moving_position = 1;
    playing_board.board_data.resize(Board_size, std::vector<int>(Board_size, 0));
    playing_board.board_data[1][1] = 1;
    playing_board.board_data[1][2] = 3;
    playing_board.board_data[1][3] = 7;
    playing_board.board_data[1][4] = 9;
    player_remain_time[0] = 100;
    player_remain_time[1] = 700;
    my_piece_type = 9;
    PP_initialize();
    PP_segement_start();
    return;
}