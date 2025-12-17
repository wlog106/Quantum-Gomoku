#include "../socket_reader.h"

void waiting_room_recv(const string &command){
    stringstream ss(command);
    waiting_room_t waiting_info;
    playing_room_t playing_info;
    int cmd_id;
    ss >> cmd_id;
    string message;
    size_t pos;
    switch (cmd_id){
        case C_new_room_info:
            if(read_waiting_info(ss, waiting_info)) return;
            lock_ui();
            apply_waiting_info(waiting_info);
            signal_ui();
            unlock_ui();
            break;
        case C_new_waiting_room_message:
            ss >> message;
            if(ss.fail())return;
            pos = 0;
            while ((pos = message.find("┼", pos)) != std::string::npos) {
                message.replace(pos, 3, " ");
            }
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
            if(read_playing_info(ss, playing_info)) return;
            set_state(S_playing);
            lock_ui();
            reset_waiting_room();
            apply_playing_info(playing_info);
            unlock_ui();
            break;
        default:
            break;
    }
    return;
}