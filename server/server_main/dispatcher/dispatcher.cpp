#include <cstdio>
#include <share_cmd.h>
#include <server_cmd.h>
#include <server_objects.h>
#include <server_utils.h>
#include <string>
#include <sys/epoll.h>
#include <assert.h>

#define MAX_ROOM 20

void dispatcher(
    ServerContext *scxt,
    ServerObjects *sobj,
    conn *u
){
    job_t *cur_job;
    while(!u->jobq.empty()){
        cur_job = u->jobq.front();
        if(cur_job->type == RES_USR){
            epoll_rw_mod(scxt->epfd, u->fd);
            break;
        }
        else if(cur_job->type == DW_SIGNUP || cur_job->type == DW_LOGIN){
            /* db_worker */
            if(sobj->dwq->empty()) 
                epoll_rw_mod(scxt->epfd, scxt->dw_fd);
            sobj->dwq->push(cur_job);
            u->jobq.pop_front();
            break;
        }
        else if(cur_job->type == CREATE_ROOM){
            job_t *newJob = new job_t;
            newJob->type = RES_USR;
            char *cmd = (char*)malloc(MAXLINE*sizeof(char));
            if(sobj->id_to_room->size() == MAX_ROOM){
                sprintf(cmd, "%d\n", C_too_much_room);
                newJob->fill_line(cmd);
                u->jobq.pop_front();
                push_res_job(u->jobq, newJob);;
            }
            else{
                std::string newRoomid = sobj->uid_gen->new_uid(ROOM_ID_LEN);
                Room *newRoom = new Room(newRoomid);
                newRoom->add_player(u);
                sprintf(cmd, "%d %s\n",
                         C_create_room_success, newRoom->get_room_info().data());
                sobj->id_to_room->insert({newRoomid, newRoom});
                newJob->fill_line(cmd);
                u->jobq.pop_front();
                push_res_job(u->jobq, newJob);;
            }
        }
        else if(cur_job->type == JOIN_ROOM_BY_ID){
            printf("searching room id %s\n", cur_job->line);
            auto it = sobj->id_to_room->find(std::string(cur_job->line));
            char *cmd = (char*)malloc(MAXLINE*sizeof(char));
            job_t *newJob = new job_t;
            newJob->type = RES_USR;
            if(it==sobj->id_to_room->end()){
                /* id DNE */
                sprintf(cmd, "%d 1\n", C_join_by_id_fail);
                newJob->fill_line(cmd);
                u->jobq.pop_front();
                push_res_job(u->jobq, newJob);;
            }
            else{
                if(!it->second->add_user(u)){
                    /* room full */
                    sprintf(cmd, "%d 2\n", C_join_by_id_fail);
                    newJob->fill_line(cmd);
                    u->jobq.pop_front();
                    push_res_job(u->jobq, newJob);;
                }
                else{
                    sprintf(cmd, "%d %s\n", 
                        C_join_by_id_success_waiting, it->second->get_room_info().data());
                    it->second->on_change(scxt->epfd, u);
                    newJob->fill_line(cmd);
                    u->jobq.pop_front();
                    push_res_job(u->jobq, newJob);;
                }
            } 
        }
        else if(cur_job->type == PAIR_RANDOMLY){
            bool flag = false;
            char *cmd = (char*)malloc(MAXLINE*sizeof(char));
            job_t *newJob = new job_t;
            newJob->type = RES_USR;
            auto it = sobj->id_to_room->begin();
            while(it!=sobj->id_to_room->end()){
                if(it->second->add_player(u)){
                    flag = true;
                    break;
                }
                it++;
            }
            if(flag){
                sprintf(cmd, "%d %s\n", 
                        C_pair_success_start_waiting, it->second->get_room_info().data());
                it->second->on_change(scxt->epfd, u);
                newJob->fill_line(cmd);
                u->jobq.pop_front();
                push_res_job(u->jobq, newJob);;
            }
            else{
                sprintf(cmd, "%d\n", C_pair_fail);
                newJob->fill_line(cmd);
                u->jobq.pop_front();
                push_res_job(u->jobq, newJob);;
            }
        }
        else if(cur_job->type == OBSERVE_RANDOMLY){

        }
        else if(cur_job->type == TOGGLE_READY){
            auto it = sobj->id_to_room->find(std::string(cur_job->line));
            if(it==sobj->id_to_room->end())
                assert(1==0);  /* error with room DNE*/
            else if(!it->second->change_ready(u))
                continue;

            if(it->second->can_fork()){
                printf("open a new playing room, pid: %d\n", 
                       fork_room(sobj, it->second, scxt->epfd));
                u->jobq.pop_front();
            }
            else{
                job_t *newJob = new job_t;
                newJob->type = RES_USR;
                char *cmd = (char*)malloc(MAXLINE*sizeof(char));
                sprintf(cmd, "%d %s\n", 
                        C_new_room_info, it->second->get_room_info().data());
                it->second->on_change(scxt->epfd, u);
                newJob->fill_line(cmd);
                u->jobq.pop_front();
                push_res_job(u->jobq, newJob);;
            }
        }
        else if(cur_job->type == CHANGE_POS){
            int pos;
            char room_id[MAXLINE];
            sscanf(cur_job->line, "%s %d", room_id, &pos);
            pos--;
            auto it = sobj->id_to_room->find(std::string(room_id));
            if(it==sobj->id_to_room->end()){
                assert(1==0);
                /* error no such room */
            }
            else if(!it->second->user_change_position(u, pos))
                continue;
                
            job_t *newJob = new job_t;
            newJob->type = RES_USR;
            char *cmd = (char*)malloc(MAXLINE*sizeof(char));
            sprintf(cmd, "%d %s\n", 
                    C_new_room_info, it->second->get_room_info().data());
            it->second->on_change(scxt->epfd, u);
            newJob->fill_line(cmd);
            u->jobq.pop_front();
            push_res_job(u->jobq, newJob);;
        }
        else if(cur_job->type == LEAVE_WAITING_ROOM){
            auto it = sobj->id_to_room->find(std::string(cur_job->line));
            if(it==sobj->id_to_room->end()){
                assert(1==0);
                /* error no such room */
            }
            else if(!it->second->user_leave(u))
                continue;

            job_t *newJob = new job_t;
            newJob->type = RES_USR;
            char *cmd = (char*)malloc(MAXLINE*sizeof(char));
            sprintf(cmd, "%d\n", C_leave_waiting_room_success);
            it->second->on_change(scxt->epfd, u);
            if(it->second->is_empty()){
                free(it->second);
                sobj->id_to_room->erase(it);
            }
            newJob->fill_line(cmd);
            u->jobq.pop_front();
            push_res_job(u->jobq, newJob);;
        }
        else if(cur_job->type == SENDMSG_WAITING_ROOM){
            char room_id[MAXLINE];
            char message[MAXLINE];
            sscanf(cur_job->line, "%s %s", room_id, message);
            auto it = sobj->id_to_room->find(std::string(room_id));
            if(it==sobj->id_to_room->end()){
                assert(1==0);
                /* error no such room */
            }
            u->jobq.pop_front();
            it->second->broadcast_msg(scxt->epfd, u, message);
        }
    }
}