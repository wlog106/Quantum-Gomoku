#include "../server_.h"
#include <cstring>

void processor(
    ServerContext *scxt, 
    conn_t *u, 
    std::queue<string> &q
){
    std::queue<job_t> jobs;
    int cmd_id = std::stoi(q.front());
    q.pop();
    switch(cmd_id)
    {
        case (C_create_new_account):
            u->name = q.front();
            q.pop();
            strcpy(u->hash, q.front().data());
            q.pop();
            jobs.push(job_t(DB_SIGNUP, u));
            break;
        case (C_login_to_server):
            u->name = q.front();
            q.pop();
            strcpy(u->hash, q.front().data());
            q.pop();
            jobs.push(job_t(DB_LOGIN, u));
            break;
    }
    dispatcher(scxt, jobs);
}