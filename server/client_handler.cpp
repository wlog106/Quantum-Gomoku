#include "server.h"
#include <cerrno>
#include <cstdio>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>

#define cur_cmd cmd_q.front()


int client_handler(db_conn *db_handler, User *user)
{
    int cmd_id;
    string cmd_info;
    queue<string> cmd_q;
    if(Read_commamd(user->sockfd, user->rbuf, cmd_q)==0) 
        return CLT_SHUTDOWN;
    while(!cmd_q.empty())
    {
        stringstream ss(cur_cmd);
        ss >> cmd_id;
        if(!validator(user->state, cmd_id)) continue;
        int n;
        if(cmd_id == C_create_new_account){
            if((n = SignUp(db_handler, ss))!=0){
                if(n == 1062){
                    printf("nickname existed\n");
                    user->wbuf = std::to_string(C_account_already_exist) + "\n\0";
                    return CLT_CAN_WRITE;
                }
                else{
                    printf("sign up fail\n");
                } 
            }
            else {
                printf("sign up success\n");
                
            }
        }
        else if(cmd_id == C_login_to_server){
            if(!Login(db_handler, ss)){
                printf("login fail\n");
            }
            else {
                printf("login success\n");
                //return user information
            }
        }
        cmd_q.pop();
    }
    return 1;
}

bool Login(db_conn *db_handler, stringstream &ss)
{
    string username, passwd_hash;
    ss >> username >> passwd_hash;
    db_get_hash(db_handler, username.data(), passwd_hash.data());
    db_handler->res_info->passwd_hash[64] = 0;
    string legit_code = db_handler->res_info->passwd_hash;
    cout << legit_code << "\n";
    return (legit_code==passwd_hash);
}

unsigned int SignUp(db_conn *db_handler, stringstream &ss)
{
    string username, passwd_hash;
    ss >> username >> passwd_hash;
    unsigned int n;
    return (db_add_user(
        db_handler, 
        username.data(), 
        passwd_hash.data()
    ));
}
