# include "../stdin_handler.h"

void select_option_std(const string &key){
    if(key == "UP"){
        if(opselect_option > 0){
            lock_ui();
            opselect_option--;
            signal_ui();
            unlock_ui();
        }
    }
    else if(key == "DOWN"){
        if(opselect_option < 3){
            lock_ui();
            opselect_option++;
            signal_ui();
            unlock_ui();
        }
    }
    else if(key == "ESC"){
        set_state(S_login_option);
        lock_ui();
        reset_opselect_ui();
        signal_ui();
        unlock_ui();
        lock_writer();
        command_to_be_sent.push(std::to_string(C_client_logout) + '\n');
        signal_writer();
        unlock_writer();
    }
    else if(key == "ENTER"){
        if(!key_press_check(key)) return;
        switch (opselect_option) {
        case 0:
            lock_ui();
            opselect_reply = OSR_waiting;
            signal_ui();
            unlock_ui();
            lock_writer();
            command_to_be_sent.push(std::to_string(C_create_room) + '\n');
            signal_writer();
            unlock_writer();
        break;
        case 1:
            set_state(S_select_enter_room_id);
            lock_ui();
            reset_opselect_ui();
            signal_ui();
            unlock_ui();
        break;
        case 2:
            lock_ui();
            opselect_reply = OSR_waiting;
            signal_ui();
            unlock_ui();
            lock_writer();
            command_to_be_sent.push(std::to_string(C_pair_randomly) + '\n');
            signal_writer();
            unlock_writer();
        break;
        case 3:
            lock_ui();
            opselect_reply = OSR_waiting;
            signal_ui();
            unlock_ui();
            lock_writer();
            command_to_be_sent.push(std::to_string(C_observe_randomly) + '\n');
            signal_writer();
            unlock_writer();
        break;
        default:
        break;
        }
    }
    return;
}

void select_enter_room_id_std(const string &key){
    if(key == "ESC"){
        set_state(S_select_option);
        lock_ui();
        reset_opselect_room_id();
        signal_ui();
        unlock_ui();
    }
    else if(key == "ENTER"){
        if(!key_press_check(key)) return;
        if(int(room_id_input_box.size()) == ROOM_ID_LEN){
            lock_ui();
            opselect_reply = OSR_waiting;
            signal_ui();
            unlock_ui();
            lock_writer();
            command_to_be_sent.push(std::to_string(C_join_room_by_id) + ' ' + room_id_input_box + '\n');
            signal_writer();
            unlock_writer(); 
        }
        else{
            lock_ui();
            opselect_reply = OSR_room_id_len_error;
            signal_ui();
            unlock_ui();
        }
    }
    else if(key == "BACKSPACE"){
        lock_ui();
        if(!room_id_input_box.empty()) room_id_input_box.pop_back();
        signal_ui();
        unlock_ui();
    }
    else if(int(key.size()) == 1){
        lock_ui();
        if(int(room_id_input_box.size()) < ROOM_ID_LEN){
            room_id_input_box += key;
        }
        signal_ui();
        unlock_ui();
    }
}