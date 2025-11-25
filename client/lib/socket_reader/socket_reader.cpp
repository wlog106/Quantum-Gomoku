#include "socket_reader.h"
#include <sys/select.h>


void *socket_reader(void *vptr){
    (void)vptr;

    int sockfd = get_sockfd();
    string remain = "";
    queue<string> commands;

    fd_set rset;
    int maxfdp1 = max(socket_reader_end_pipe[0], sockfd) + 1;
    State_t state;
    int n;
    int has_end = 0;//end if read 0
    while(true){
        
        FD_ZERO(&rset);
        FD_SET(socket_reader_end_pipe[0], &rset);
        if(!has_end) FD_SET(sockfd, &rset);
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
            n= Read_commamd(sockfd, remain, commands);
            if(n == 0){
                CLOSE_CLIENT('s');
                has_end = 1;
                continue;
            }
            /*process command*/
            while(commands.size()){
                state = get_state();
                string &command = commands.front();
                switch (state) {
                    case S_login_with_account:
                        login_with_account_recv(command);
                    case S_creating_account:
                        creating_account_recv(command);
                    default:
                    break;
                }
                commands.pop();
            }
            
        }
    }
    return NULL;
}