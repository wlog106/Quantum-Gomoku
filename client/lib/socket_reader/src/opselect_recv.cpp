#include "../socket_reader.h"

void select_option_recv(const string &command){
    /** 
    result of:
    create room
    pair randomly
    observe randomly
    */
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id){
        //create room
        case C_create_room_success:
            read_room_info(ss);
            set_state(S_waiting_room);
            lock_ui();
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
            read_room_info(ss);
            set_state(S_waiting_room);
            lock_ui();
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
        case C_start_observing:
            break;
        case C_no_current_playing_room:
            lock_ui();
            opselect_reply = OSR_no_current_playing_room;
            signal_ui();
            unlock_ui();
            break;
        default:
            break;
    }
}

void select_enter_room_id_recv(const string &command){
    //result of join by room id
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    switch (cmd_id){
        case C_join_by_id_success_waiting:
            read_room_info(ss);
            set_state(S_waiting_room);
            lock_ui();
            reset_opselect_room_id();
            signal_ui();
            unlock_ui();
            break;
        case C_join_by_id_success_playing:
            break;
        case C_join_by_id_fail:
            int reason;
            ss >> reason;
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

void read_room_info(stringstream &ss){
    lock_ui();
    ss >> waiting_room_id;
    for(int i = 0; i < 5; i++) ss >> waiting_user_existance[i];
    for(int i = 0; i < 2; i++) ss >> waiting_is_ready[i];
    for(int i = 0; i < 5; i++){
        if(waiting_user_existance[i]){
            ss >> waiting_username[i];
        }
    }
    unlock_ui();
}