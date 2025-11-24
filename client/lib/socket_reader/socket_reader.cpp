#include "socket_reader.h"
#include <sys/select.h>


void *socket_reader(void *vptr){
    (void)vptr;
    return NULL;

    string remain = "";
    queue<string> commands;

    fd_set rset;
    int maxfdp1 = max(socket_reader_end_pipe[0], sockfd) + 1;
    int state;
    while(true){
        
        FD_ZERO(&rset);
        FD_SET(socket_reader_end_pipe[0], &rset);
        FD_SET(sockfd, &rset);
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
            while(commands.size()){
                state = get_state();
                string command = commands.front();
                commands.pop();
                switch (state) {
                    case S_login_with_account:

                    default:
                    break;
                }
            }
            
        }
    }
    return NULL;
}