#include "../server_.h"

#include <sys/wait.h>

void sigchild(int signo)
{
    pid_t pid;
    int stat;

    while((pid=waitpid(-1, &stat, WNOHANG)) > 0){
        cout << "room with pid: " << pid << "terminated\n";
    }
    return;
}