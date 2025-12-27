#include "../socket_reader.h"
#include "share_cmd.h"

void select_option_recv(const string &command){
    /** 
    result of:
    create room
    pair randomly
    observe randomly
    */
    stringstream ss(command);
    waiting_room_t waiting_info;
    playing_room_t playing_info;
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id){
        //create room
        case C_create_room_success:
            if(read_waiting_info(ss, waiting_info))return;
            set_state(S_waiting_room);
            lock_ui();
            apply_waiting_info(waiting_info);
            reset_opselect_ui();
            signal_ui();
            unlock_ui();
            break;
        case C_too_much_room:
            lock_ui();
            opselect_reply = OSR_too_much_room;
            signal_ui();
            unlock_ui();
            break;
        //pair randomly
        case C_pair_success_start_waiting:
            if(read_waiting_info(ss, waiting_info))return;
            set_state(S_waiting_room);
            lock_ui();
            apply_waiting_info(waiting_info);
            reset_opselect_ui();
            signal_ui();
            unlock_ui();
            break;
        case C_pair_fail:
            lock_ui();
            opselect_reply = OSR_no_empty_waiting_room;
            signal_ui();
            unlock_ui();
            break;
        //observe randomly
        case C_no_current_playing_room:
            lock_ui();
            opselect_reply = OSR_no_current_playing_room;
            signal_ui();
            unlock_ui();
            break;
        case C_start_a_playing_room:
            if(read_playing_info(ss, playing_info)) return;
            set_state(S_playing);
            lock_ui();
            apply_playing_info(playing_info);
            reset_opselect_ui();
            unlock_ui();
            break;
        default:
            break;
    }
}

void select_enter_room_id_recv(const string &command){
    //result of join by room id
    stringstream ss(command);
    waiting_room_t waiting_info;
    playing_room_t playing_info;
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id){
        case C_join_by_id_success_waiting:
            if(read_waiting_info(ss, waiting_info))return;
            set_state(S_waiting_room);
            lock_ui();
            apply_waiting_info(waiting_info);
            reset_opselect_room_id();
            signal_ui();
            unlock_ui();
            break;
        case C_start_a_playing_room:
            if(read_playing_info(ss, playing_info)) return;
            set_state(S_playing);
            lock_ui();
            apply_playing_info(playing_info);
            reset_opselect_room_id();
            unlock_ui();
            break;
        case C_join_by_id_fail:
            int reason;
            ss >> reason;
            if(ss.fail())return;
            lock_ui();
            switch (reason) {
                case 1:
                    opselect_reply = OSR_room_id_dne;
                    break;
                case 2:
                    opselect_reply = OSR_room_already_full;
                    break;
                default:
                    break;
            }
            signal_ui();
            unlock_ui();
            break;
        default:
            break;
    }
}