#include "universal.h"
using std::cout;
using std::queue;
using std::string;

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr){
    int n;

again:
    if( (n = TEMP_FAILURE_RETRY(accept(fd, sa, salenptr))) < 0 ){
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
    if(TEMP_FAILURE_RETRY(bind(fd, sa, salen)) < 0){
        cout << "bind error\n";
        exit(1);
    }
}

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

void Listen(int fd, int backlog){
    if(TEMP_FAILURE_RETRY(listen(fd, backlog)) < 0){
        cout << "listen error\n";
        exit(1);
    }
}

int Read_commamd(int fd, queue<string> &buf, queue<string> &result){

    int has_one_command = 0;

    size_t n;
    char read_buf[MAXLINE];
    if ((n = read(fd, read_buf, MAXLINE)) == -1){
        cout << "read error\n";
        exit(0);
    }
    string one_block = "";
    for(int i = 0; i < n; i++){
        if(read_buf[i] == ' '){
            buf.push(one_block);
            one_block = "";
        }
        else if(read_buf[i] == '\n'){
            if(one_block != "") buf.push(one_block);
            buf.push("\n");
        }
        else{
            one_block += read_buf[i];
        }
    }
    if(one_block != "") buf.push(one_block);

    while(buf.size()){
        string tmp = buf.front();
        if(tmp == "\n"){
            has_one_command = 1;
            break;
        }
        else{
            result.push(tmp);
        }
    }

    if(has_one_command){
        return 1;
    }
    else{
        swap(buf, result);
        return 0;
    }
}

Sigfunc *Signal(int signo, Sigfunc *sighandler){
    struct sigaction  action, original_action;
    action.sa_handler = sighandler;
    sigemptyset(&action.sa_mask);  /* don't mask other signal */
    action.sa_flags |= SA_RESTART;
    if(sigaction(signo, &action, &original_action) < 0){
        cout << "fail to enroll signal handler\n";
        exit(1);
    }
    return original_action.sa_handler;
}

int Socket(int family, int type, int protocol){
    int n;
    if( (n = TEMP_FAILURE_RETRY(socket(family, type, protocol))) < 0){
        cout << "socket error\n";
        exit(1);
    }
    return n;
}

