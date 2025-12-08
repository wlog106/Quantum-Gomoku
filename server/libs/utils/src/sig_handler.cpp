#include <stdio.h>
#include <sys/wait.h>

void sigchild(int signo)
{
    pid_t pid;
    int stat;

    while((pid=waitpid(-1, &stat, WNOHANG)) > 0){
        printf("room with pid: %d terminated\n", pid);
    }
    return;
}