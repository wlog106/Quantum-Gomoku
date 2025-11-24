#include "socket_writer.h"
#include <sys/select.h>


void *socket_writer(void *vptr){
    (void)vptr;

    int sockfd = get_sockfd();
    fd_set rset, wset;
    int maxfdp1 = max(sockfd, socket_writer_end_pipe[0]) + 1;
    while(true){
        
        FD_ZERO(&rset);
        FD_SET(socket_writer_end_pipe[0], &rset);
        FD_ZERO(&wset);
        FD_SET(sockfd, &wset);
        Select(maxfdp1, &rset, &wset, nullptr, nullptr);
        if(FD_ISSET(socket_writer_end_pipe[0], &rset)){
            char ch;
            Read(socket_writer_end_pipe[0], &ch, 1);
            break;
        }
        if(FD_ISSET(sockfd, &wset)){
            lock_writer();

            if(int(command_to_be_sent.size()) == 0){
                wait_writer();
            }
            if(writer_end){
                unlock_writer();
                break;
            }
            
            char sendline[MAXLINE];
            string command = command_to_be_sent.front();
            command_to_be_sent.pop();

            strcpy(sendline, command.c_str());

            Write(sockfd, sendline, strlen(sendline));
            unlock_writer();
        }
    }
    return NULL;
}