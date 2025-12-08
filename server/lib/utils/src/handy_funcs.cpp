#include <map>

struct conn;

conn *get_user(
    int fd, 
    std::map<int, conn*> *fd_to_conn
){
    auto it = fd_to_conn->find(fd);
    return it->second;
}