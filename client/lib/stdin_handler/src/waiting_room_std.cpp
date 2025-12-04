# include "../stdin_handler.h"

void waiting_room_std(const string &key){
    int is_entering;
    lock_ui();
    is_entering = waiting_room_entering;
    unlock_ui();
    if(is_entering){
        if(key == "ENTER"){
            lock_ui();
            string message = waiting_room_input_box;
            waiting_room_input_box = "";
            waiting_room_entering = ((int(message.size()) != 0) ? 1 : 0);
            signal_ui();
            unlock_ui();
            if(int(message.size()) != 0){
                if(!key_press_check(key)) return;
                lock_writer();
                command_to_be_sent.push(std::to_string(C_send_message_waiting_room) + " " + message + "\n");
                signal_writer();
                unlock_writer();
            }
        }
        else if(key == "BACKSPACE"){
            lock_ui();
            if(!waiting_room_input_box.empty()) waiting_room_input_box.pop_back();
            signal_ui();
            unlock_ui();
        }
        else if(int(key.size()) == 1){
            lock_ui();
            if(int(waiting_room_input_box.size()) < MAX_MESSAGE_LEN){
                waiting_room_input_box += key;
            }
            signal_ui();
            unlock_ui();
        }
    }
    else{
        if(key == "ESC"){
            if(!key_press_check(key)) return;
            lock_writer();
            command_to_be_sent.push(std::to_string(C_leave_waiting_room) + "\n");
            signal_writer();
            unlock_writer();
        }
        else if(key == "R" || key == "r"){
            if(!key_press_check(key)) return;
            lock_writer();
            command_to_be_sent.push(std::to_string(C_change_ready) + "\n");
            signal_writer();
            unlock_writer();
        }
        else if(key == "1" || key == "2" || key == "3" || key == "4" || key == "5"){
            if(!key_press_check(key)) return;
            lock_ui();
            bool has_player = waiting_user_existance[key[0]-'1'];
            unlock_ui();
            if(has_player) return;
            lock_writer();
            command_to_be_sent.push(std::to_string(C_change_waiting_position) + " " + key + "\n");
            signal_writer();
            unlock_writer();
        }
        else if(key == "ENTER"){
            lock_ui();
            waiting_room_entering = 1;
            signal_ui();
            unlock_ui();
        }
    }
}