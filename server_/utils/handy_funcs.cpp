#include "../server_.h"

#include <map>

conn_t *get_user(
    int fd, 
    std::map<int, conn_t*> &fd_to_conn
){
    auto it = fd_to_conn.find(fd);
    return it->second;
}