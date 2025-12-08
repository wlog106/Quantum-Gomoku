#include "../socket_reader.h"

void waiting_room_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    string message;
    switch (cmd_id){
        case C_new_room_info:
            read_room_info(ss);
            lock_ui();
            signal_ui();
            unlock_ui();
            break;
        case C_new_waiting_room_message:
            ss >> message;
            lock_ui();
            waiting_room_history_message.push_back(message);
            if(int(waiting_room_history_message.size()) > 5){
                waiting_room_history_message.pop_front();
            }
            signal_ui();
            unlock_ui();
            break;
        case C_leave_waiting_room_success:
            set_state(S_select_option);
            lock_ui();
            reset_waiting_room();
            signal_ui();
            unlock_ui();
            break;
        case C_start_a_playing_room:
            set_state(S_playing);
            lock_ui();
            reset_waiting_room();
            read_playing_info(ss);
            PP_initialize();
            unlock_ui();
            break;
        default:
            break;
    }
    return;
}