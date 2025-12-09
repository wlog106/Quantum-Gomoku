#include <server_objects.h>

ServerObjects::ServerObjects(){}

ServerObjects::ServerObjects(
    std::queue<job_t*> *dwq,
    linear_buf_t *dwr_buf,
    std::map<int, conn*> *fd_to_conn,
    std::map<int, Room*> *id_to_room
){
    this->dwq = dwq;
    this->dwr_buf = dwr_buf;
    this->fd_to_conn = fd_to_conn;
    this->id_to_room = id_to_room;
}