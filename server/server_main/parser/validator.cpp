#include <share_cmd.h>
#include <server_cmd.h>

bool validator(
    int state,
    int cmd_id
){
    switch(state)
    {
        case (UNKNOWN_USR):
            if(cmd_id==C_create_new_account)
                return true;
            if(cmd_id==C_login_to_server)
                return true;
            if(cmd_id==C_client_logout)
                return true;
            return false;
            
        case (OP_SELECTING_USR):
            if(cmd_id==C_create_room)
                return true;
            if(cmd_id==C_join_room_by_id)
                return true;
            if(cmd_id==C_pair_randomly)
                return true;
            if(cmd_id==C_observe_randomly)
                return true;
            return false;
            
        case (WAITING_ROOM_USR):
            if(cmd_id==C_change_ready)
                return true;
            if(cmd_id==C_change_waiting_position)
                return true;
            if(cmd_id==C_leave_waiting_room)
                return true;
            if(cmd_id==C_send_message_waiting_room)
                return true;
            return false;
            
        case (PLAYING_ROOM_USR):
            return false;
    }
    return false;
}