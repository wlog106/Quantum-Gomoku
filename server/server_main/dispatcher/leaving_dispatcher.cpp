#include "share_cmd.h"
#include <server_objects.h>
#include <server_utils.h>
#include <server_cmd.h>

void leaving_dispatcher(
    ServerObjects *sobj,
    conn *u,
    int cmd
){
    if(u->state==WAITING_ROOM_USR){
        auto it = sobj->id_to_room->find(u->room_id);
        if(it==sobj->id_to_room->end()){
            fprintf(stderr, 
                    "Internal error: user in waiting room but can't find corresponding room id.\n");
            exit(1);
        }
        it->second->user_leave(u);
        if(it->second->is_empty()){
            delete it->second;
            sobj->id_to_room->erase(it);
            printf("room empty... delete room\n");
        }
    }
    sobj->fd_to_conn->erase(u->fd);
    sobj->login_ids->erase(u->id);
    if(cmd==C_client_logout){
        conn *newConn = new conn(u->fd, UNKNOWN_USR);
        sobj->fd_to_conn->insert({u->fd, newConn});
    }
    else{
        Close(u->fd);
    }
    printf("user: %s leave successfully\n", u->name);
    delete u;
}