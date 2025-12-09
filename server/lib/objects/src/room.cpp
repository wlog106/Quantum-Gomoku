#include <server_objects.h>

Room::Room(std::string rm_id):room_id(rm_id){
    for(int i = 0; i < 5; i++){
        user_name[i] = "";
        user_existance[i] = false;
    }
    user_ready[0] = 0;
    user_ready[1] = 0;
}

bool Room::add_user(string new_user){
    bool has_add = false;
    for(int i = 0; i < 5; i++){
        if(user_existance[i] == 0){
            has_add = true;
            user_existance[i] = true;
            user_name[i] = new_user;
            break;
        } 
    }
    return has_add;
}

bool Room::user_leave(string leaving_user){
    bool has_leave = false;
    for(int i = 0; i < 5; i++){
        if(user_existance[i] && user_name[i] == leaving_user){
            has_leave = true;
            user_existance[i] = false;
            user_name[i] = "";
            break;
        }
    }
    return has_leave;
}

bool Room::user_change_position(string user, int pos){
    if(pos < 1 || pos > 5) return false;
    bool has_change = false;
    for(int i = 0; i < 5; i++){
        if(user_existance[i] == 1 &&
           user_name[i] == user   &&
           user_existance[pos] == 0){
            has_change = true;
            user_existance[i] = 0;
            user_name[i] = "";
            user_existance[pos] = 1;
            user_name[pos] = user;
            break;
        }
    }
    return has_change;
}
    
bool Room::change_ready(string user){
    bool has_change = false;
    for(int i = 0; i < 2; i++){
        if(user_existance[i] && user_name[i] == user){
            has_change = true;
            user_ready[i] ^= true;
            break;
        }
    }
    return has_change;
}

string Room::get_room_info(){
    string reply = (room_id + " ");
    for(int i = 0; i < 5; i++){
        reply += (std::to_string(user_existance[i]) + " ");
    }
    reply += (std::to_string(user_ready[0]) + " ");
    reply += (std::to_string(user_ready[1]) + " ");
    for(int i = 0; i < 5; i++){
        if(user_existance[i]){
            reply += (user_name[i] + " ");
        }
    }
    reply.pop_back();
    reply += "\n";
    return reply;
}