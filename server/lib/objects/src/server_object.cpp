#include <server_objects.h>

ServerObjects::ServerObjects(){}

ServerObjects::ServerObjects(
    std::queue<job_t*> *dwq,
    linear_buf_t *dwr_buf,
    Uid_generator *uid_gen,
    std::map<int, conn*> *fd_to_conn,
    std::set<int> *playing_room_fds,
    std::map<std::string, Room*> *id_to_room,
    std::set<int> *login_ids
){
    this->dwq = dwq;
    this->dwr_buf = dwr_buf;
    this->uid_gen = uid_gen;
    this->fd_to_conn = fd_to_conn;
    this->playing_room_fds = playing_room_fds;
    this->id_to_room = id_to_room;
    this->login_ids = login_ids;
}