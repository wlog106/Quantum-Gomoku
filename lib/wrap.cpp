#include "universal.h"


int Socket(int family, int type, int protocol){
    int n;
    if( (n = socket(family, type, protocol)) < 0){
        cout << "socket error\n";
        exit(1);
    }
    return n;
}

void Inet_pton(int family, const char *strptr, void *addrptr){
    int n;
    if( (n = inet_pton(family, strptr, addrptr)) < 0){
        cout << "(arg0) address family error\n";
        exit(1);
    }
    else if(n==0){
        cout << "(arg1) presentation error or doesn't match given address family\n";
        exit(1);
    }
}