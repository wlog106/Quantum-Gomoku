#include "universal.h"

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr){
    int n;

again:
    if( (n = accept(fd, sa, salenptr)) < 0 ){
        // ECONNABORTED: client abort before accept
#ifdef EPROTO
        if(errno == EPROTO || errno == ECONNABORTED)
#else
        if(errno == ECONNABORTED)
#endif     
            goto again;
        else{
            cout << "accept error\n";
            exit(1);
        }
    }
    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen){
    if(bind(fd, sa, salen) < 0){
        cout << "bind error\n";
        exit(1);
    }
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen){
    if(connect(fd, sa, salen) < 0){
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

void Listen(){
    
}

int Socket(int family, int type, int protocol){
    int n;
    if( (n = socket(family, type, protocol)) < 0){
        cout << "socket error\n";
        exit(1);
    }
    return n;
}

