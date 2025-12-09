#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <share_wrap.h>

#include <iostream>

void Connect(int fd, const struct sockaddr *sa, socklen_t salen){
    if(TEMP_FAILURE_RETRY(connect(fd, sa, salen)) < 0){
        std::cout << "connect error\n";
        exit(1);
    }
}

int Fcntl(int fd, int cmd, int arg){
	int	n;
	if ( (n = fcntl(fd, cmd, arg)) < 0)
		std::cout << "fcntl error\n";
	return n;
}

void Inet_pton(int family, const char *strptr, void *addrptr){
    int n;
    if( (n = inet_pton(family, strptr, addrptr)) < 0){
        std::cout << "(arg0) address family error\n";
        exit(1);
    }
    else if(n == 0){
        std::cout << "(arg1) presentation error or doesn't match given address family\n";
        exit(1);
    }
}

ssize_t Read(int fd, void *ptr, size_t nbytes){
	ssize_t	n;
	if((n = read(fd, ptr, nbytes)) == (ssize_t)-1){
        std::cout << "read error" << endl;
        exit(1);
    }
    return n;
}

int Read_commamd(int fd, string &buf, queue<string> &commands){

    int n;
    char read_buf[MAXLINE];
    if ((n = TEMP_FAILURE_RETRY(read(fd, read_buf, MAXLINE))) == -1){
        //std::cout << "haharead error\n";
        return 0;
    }
    read_buf[n] = 0;
    buf += read_buf;

    stringstream ss(buf);
    string command;
    while(std::getline(ss, command, '\n')){
        if(!ss.eof()){
            commands.push(command);
        }
    }
    buf = command;
    return n;
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
	int n;
	if((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0){
        std::cout << "select error" << endl;
        exit(0);
    }
	return(n);
}

void Shutdown(int fd, int how) {
	if (shutdown(fd, how) < 0){
        std::cout << "shutdown error" << endl;
    }
}

int Socket(int family, int type, int protocol){
    int n;
    if( (n = TEMP_FAILURE_RETRY(socket(family, type, protocol))) < 0){
        std::cout << "socket error\n";
        exit(1);
    }
    return n;
}

void Pipe(int* pipedes){
    if(pipe(pipedes) < 0){
        std::cout << "pipe error\n";
        exit(1);
    }
}

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void * (*func)(void *), void *arg){
	int	n;
	if ((n = pthread_create(tid, attr, func, arg)) == 0) return;
	errno = n;
	std::cout << "pthread_create error" << endl;
    exit(1);
}

void Pthread_cond_signal(pthread_cond_t *cptr){
	int	n;
	if((n = pthread_cond_signal(cptr)) == 0) return;
	errno = n;
	std::cout << "pthread_cond_signal error" << endl;
    exit(1);
}

void Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr){
	int	n;
	if((n = pthread_cond_wait(cptr, mptr)) == 0) return;
	errno = n;
	std::cout << "pthread_cond_wait error" << endl;
    exit(1);
}

void Pthread_join(pthread_t tid, void **status){
	int	n;
	if((n = pthread_join(tid, status)) == 0) return;
	errno = n;
	std::cout << "pthread_join error" << endl;
    exit(1);
}

void Pthread_mutex_lock(pthread_mutex_t *mptr) {
	int	n;
	if((n = pthread_mutex_lock(mptr)) == 0)return;
	errno = n;
	std::cout << "pthread_mutex_lock error" << endl;
    exit(1);
}

void Pthread_mutex_unlock(pthread_mutex_t *mptr){
	int	n;
	if((n = pthread_mutex_unlock(mptr)) == 0) return;
	errno = n;
	std::cout << "pthread_mutex_unlock error" << endl;
    exit(1);
}

void Write(int fd, const void *ptr, size_t nbytes){
    size_t n;
	if ((n = TEMP_FAILURE_RETRY(write(fd, ptr, nbytes))) != nbytes){
		std::cout << "write error\n";
        exit(1);
    }
}