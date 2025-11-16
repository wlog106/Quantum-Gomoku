#include "../lib/universal.h"
#include <sys/socket.h>

int main(int argc, char **argv){

    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TEST_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
}