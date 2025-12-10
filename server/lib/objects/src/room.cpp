#include "server_cmd.h"
#include "server_utils.h"
#include "share_cmd.h"
#include "share_wrap.h"
#include <cstddef>
#include <server_objects.h>

Room::Room(std::string rm_id):room_id(rm_id){
    for(int i = 0; i < 5; i++){
        users[i] = NULL;
        user_existance[i] = false;
    }
    user_ready[0] = 0;
    user_ready[1] = 0;
}

bool Room::add_user(conn* new_user){
    bool has_add = false;
    for(int i = 0; i < 5; i++){
        if(user_existance[i] == 0){
            has_add = true;
            user_existance[i] = true;
            users[i] = new_user;
            break;
        } 
    }
    return has_add;
}

bool Room::add_player(conn* new_user){
    bool has_add = false;
    for(int i = 0; i < 2; i++){
        if(user_existance[i] == 0){
            has_add = true;
            user_existance[i] = true;
            users[i] = new_user;
            break;
        } 
    }
    return has_add;
}

bool Room::add_observer(conn* new_user){
    bool has_add = false;
    for(int i = 2; i < 5; i++){
        if(user_existance[i] == 0){
            has_add = true;
            user_existance[i] = true;
            users[i] = new_user;
            break;
        } 
    }
    return has_add;
}

bool Room::user_leave(conn* leaving_user){
    bool has_leave = false;
    for(int i = 0; i < 5; i++){
        if(user_existance[i] 
        && strcmp(users[i]->name, leaving_user->name)==0){
            has_leave = true;
            user_existance[i] = false;
            users[i] = leaving_user;
            break;
        }
    }
    return has_leave;
}

bool Room::user_change_position(conn* u, int pos){
    if(pos < 1 || pos > 5) return false;
    bool has_change = false;
    for(int i = 0; i < 5; i++){
        if(user_existance[i] == 1
        && users[i] == u
        && user_existance[pos] == 0){
            has_change = true;
            user_existance[i] = 0;
            users[i] = NULL;
            user_existance[pos] = 1;
            users[pos] = u;
            break;
        }
    }
    return has_change;
}

void Room::on_change(ServerContext *scxt, conn *u){
    for(int i = 0; i < 5; i++){
        if(!user_existance[i] || users[i] == u)
            continue;
        job_t *newJob = new job_t;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        newJob->type = RES_USR;
        sprintf(cmd, "%d %s\n",
                C_new_room_info, this->get_room_info().data());
        newJob->fill_line(cmd);
        users[i]->jobq.push_front(newJob);
        /*
            The other users' dispatcher may not be called
            (i.e. won't be add into EPOLLOUT). Thus, add
            other users' into EPOLLOUT here manually.
        */
        epoll_rw_mod(scxt, users[i]->fd);
    }
}
    
bool Room::change_ready(conn *u){
    bool has_change = false;
    for(int i = 0; i < 2; i++){
        if(user_existance[i]
        && users[i] == u){
            has_change = true;
            user_ready[i] ^= true;
            break;
        }
    }
    return has_change;
}

std::string Room::get_room_info(){
    std::string reply = (room_id + " ");
    for(int i = 0; i < 5; i++){
        reply += (std::to_string(user_existance[i]) + " ");
    }
    reply += (std::to_string(user_ready[0]) + " ");
    reply += (std::to_string(user_ready[1]) + " ");
    for(int i = 0; i < 5; i++){
        if(user_existance[i]){
            reply += (std::string(users[i]->name) + " ");
        }
    }
    reply.pop_back();
    return reply;
}