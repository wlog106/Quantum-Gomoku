#include "server.h"
#include <cerrno>
#include <cstdio>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>

#define cur_cmd cmd_q.front()
using std::string;
using std::queue;

void client_handler(db_conn *db_handler, User &user)
{
    int cmd_id;
    string cmd_info;
    queue<string> cmd_q;
    Read_commamd(user.sockfd, user.rbuf, cmd_q);
    while(!cmd_q.size())
    {
        split_cmd(cur_cmd, &cmd_id, cmd_info);
        if(!validator(user.state, cmd_id)) continue;
        if(cmd_id == C_create_new_account){
            if(!SignUp(db_handler, cmd_info)){
                printf("sign up fail\n");
            }
            else {
                printf("sign up success\n");
                //return user information
            }
        }
        else if(cmd_id == C_login_to_server){
            if(!Login(db_handler, cmd_info)){
                printf("login fail\n");
            }
            else {
                printf("login success\n");
                //return user information
            }
        }
        cmd_q.pop();
    }
}

bool Login(db_conn *db_handler, string &str)
{
    auto pos = str.find(' ');
    string username = str.substr(0, pos);
    string passwd_hash = str.substr(pos).c_str();
    db_get_hash(db_handler, username.data(), passwd_hash.data());
    string legit_code = db_handler->res_info->passwd_hash;
    return (legit_code==passwd_hash);
}

bool SignUp(db_conn *db_handler, string &str)
{
    auto pos = str.find(' ');
    string username = str.substr(0, pos);
    string passwd_hash = str.substr(pos);
    return (db_add_user(db_handler, username.data(), passwd_hash.data())==0);
}
