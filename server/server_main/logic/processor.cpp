#include <universal.h>
#include <command.h>
#include <server_objects.h>
#include <server_utils.h>
#include <server_cmd.h>

#include <sys/epoll.h>

void processor(
    ServerContext *scxt, 
    ServerObjects *sobj,
    conn *u, 
    std::queue<std::pair<int, char*>> &q
){
    while(!q.empty()){
        job_t *newJob = new job_t;
        char *cmd = (char*)malloc(MAXLINE*sizeof(char));
        switch(q.front().first)
        {
            case (C_create_new_account):
                sscanf(q.front().second, "%s %s", u->name, u->hash);
                u->db_job_pending = 1;
                newJob->type = DW_SIGNUP;
                sprintf(cmd, "%d %d %s\n", DW_SIGNUP, u->fd, q.front().second);
                newJob->line = cmd;
                newJob->r_ptr = cmd;
                newJob->len = strlen(cmd);
                sobj->dwq->push(newJob);
                if(sobj->dwq->size()==1) goto add_dw_to_epoll;
                break;
                
            case (C_login_to_server):
                sscanf(q.front().second, "%s %s", u->name, u->hash);
                u->db_job_pending = 1;
                newJob->type = DW_LOGIN;
                sprintf(cmd, "%d %d %s\n", DW_LOGIN, u->fd, q.front().second);
                newJob->line = cmd;
                newJob->r_ptr = cmd;
                newJob->len = strlen(cmd);
                sobj->dwq->push(newJob);
                if(sobj->dwq->size()==1) goto add_dw_to_epoll;
                break;

            add_dw_to_epoll:
                struct epoll_event ev;
                ev.events = EPOLLOUT;
                ev.data.fd = scxt->dw_fd;
                Epoll_ctl(scxt->epfd, EPOLL_CTL_MOD, scxt->dw_fd, &ev);
        }
        q.pop();
    }
}