#include <server_utils.h>
#include <server_objects.h>
#include <assert.h>

void room_msg_dispatcher(
    ServerContext *scxt,
    ServerObjects *sobj,
    char *room_msg,
    int fd
){
    std::string room_id;
    char *name;
    if(*room_msg=='0'){
        printf("recv room msg: %s\n", room_msg);
        *(room_msg+8) = 0;
        room_id = std::string(room_msg+2);
        name = room_msg+9;
        printf("now user: %s with fd: %d, leaving room: %s\n", name, fd, room_id.data());
        auto it = sobj->id_to_room->find(room_id);
        Room *cur_room = it->second;
        conn *cur_conn = NULL;
        for(int i=0; i<5; i++){
            if(cur_room->users[i]!=NULL
            && cur_room->users[i]->fd == -1
            && cur_room->user_existance[i]
            && strcmp(cur_room->users[i]->name, name)==0)
            {
                cur_conn = cur_room->users[i];
            }
        }
        if(cur_conn==NULL){
            printf("no such user\n");
        }
        else{
            cur_room->user_leave(cur_conn);
            cur_conn->fd = fd;
            epoll_r_add(scxt->epfd, fd);
            sobj->fd_to_conn->insert({fd, cur_conn});
            printf("recovering user fd and leaving room success\n");
        }
    }
    else if(*room_msg=='1'){

    }
    else assert(1==0);
}