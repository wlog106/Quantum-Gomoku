#include "universal.h"
#include <sys/epoll.h>
using std::cout;
using std::queue;
using std::string;
using std::stringstream;

void Connect(int fd, const struct sockaddr *sa, socklen_t salen){
    if(TEMP_FAILURE_RETRY(connect(fd, sa, salen)) < 0){
        cout << "connect error\n";
        exit(1);
    }
}

void Inet_pton(int family, const char *strptr, void *addrptr){
    int n;
    if( (n = inet_pton(family, strptr, addrptr)) < 0){
        cout << "(arg0) address family error\n";
        exit(1);
    }
    else if(n == 0){
        cout << "(arg1) presentation error or doesn't match given address family\n";
        exit(1);
    }
}

int Read_commamd(int fd, string &buf, queue<string> &commands){

    int command_count = 0;

    size_t n;
    char read_buf[MAXLINE];
    if ((n = TEMP_FAILURE_RETRY(read(fd, read_buf, MAXLINE))) == -1){
        cout << "read error\n";
        exit(0);
    }
    buf += read_buf;

    stringstream ss(buf);
    string command;
    while(std::getline(ss, command, '\n')){
        if(!ss.eof()){
            commands.push(command);
            command_count++;
        }
    }
    buf = command;
    return command_count;
}

int Socket(int family, int type, int protocol){
    int n;
    if( (n = TEMP_FAILURE_RETRY(socket(family, type, protocol))) < 0){
        cout << "socket error\n";
        exit(1);
    }
    return n;
}

void Write(int fd, void *ptr, size_t nbytes){
	if (TEMP_FAILURE_RETRY(write(fd, ptr, nbytes)) != nbytes){
		cout << "write error\n";
        exit(1);
    }
}