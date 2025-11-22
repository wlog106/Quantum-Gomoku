#include "socket_reader.h"
#include <sys/select.h>


void *socket_reader(void *vptr){
    (void)vptr;
    return NULL;

    string remain = "";
    queue<string> commands;

    while(true){
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(socket_reader_end_pipe[0], &rset);
        FD_SET(sockfd, &rset);
        int maxfdp1 = max(socket_reader_end_pipe[0], sockfd) + 1;
        Select(maxfdp1, &rset, nullptr, nullptr, nullptr);
        if(FD_ISSET(socket_reader_end_pipe[0], &rset)){
            /*
            termination of socket writer
            */
            lock_writer();
            writer_end = 1;
            signal_writer();
            unlock_writer();
            break;
        }
        if(FD_ISSET(sockfd, &rset)){
            Read_commamd(sockfd, remain, commands);
            /*process command*/
        }
    }
    return NULL;
}