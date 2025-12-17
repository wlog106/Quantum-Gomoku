#include <cstdlib>
#include <server_cmd.h>
#include <cstdio>
#include <server_utils.h>
#include <server_objects.h>
#include <assert.h>

void room_msg_dispatcher(
    ServerContext *scxt,
    ServerObjects *sobj,
    char *room_msg,
    int fd
){
    int op;
    char room_id[10];
    char name[50];
    int elo;
    printf("recv room msg: %s\n", room_msg);
    sscanf(room_msg, "%d %s %s %d", &op, room_id, name, &elo);
    printf("now user: %s with fd: %d, leaving room: %s\n", name, fd, room_id);
    auto it = sobj->id_to_room->find(std::string(room_id));
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
        if(cur_conn->cur_elo != elo){
            job_t *newDwJob = new job_t;
            newDwJob->type = DW_SET_ELO;
            char *cmd = (char*)malloc(100*sizeof(char));
            /* op id new_elo */
            sprintf(cmd, "%d %d %d\n",
                    DW_SET_ELO, cur_conn->id, elo);
            newDwJob->fill_line(cmd);
            sobj->dwq->push(newDwJob);
            epoll_rw_mod(scxt->epfd, scxt->dw_fd);
            printf("update %s elo from %d to %d\n",
                   cur_conn->name, cur_conn->cur_elo, elo);
            cur_conn->cur_elo = elo;
        }
        if(op==0){
            cur_room->user_leave(cur_conn);
            if(cur_room->is_empty()){
                sobj->id_to_room->erase(cur_room->room_id);
                delete cur_room;
                printf("room empty... delete room\n");
            }
            cur_conn->fd = fd;
            cur_conn->state = OP_SELECTING_USR;
            epoll_r_add(scxt->epfd, fd);
            sobj->fd_to_conn->insert({fd, cur_conn});
            printf("recovering user fd and leaving room success\n"); 
        }
        else if(op==1){
            sobj->login_ids->erase(cur_conn->id);
            cur_room->user_leave(cur_conn);
            if(cur_room->is_empty()){
                sobj->id_to_room->erase(cur_room->room_id);
                delete cur_room;
                printf("room empty... delete room\n");
            }
            close(fd);
            delete cur_conn;
            printf("delete pathetic user successfully\n");
        }
        else assert(1==0);
    }
}