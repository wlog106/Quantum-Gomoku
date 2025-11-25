#include "server.h"
#include <cerrno>
#include <cstdio>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>

#define cur_cmd cmd_q.front()
#define PUT_MSG(msg) std::to_string(msg) + "\n" 

int client_handler(db_conn *db_handler, User *user, set<string> &login_user)
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
            if((n = SignUp(db_handler, ss, user))!=0){
                if(n == 1062){
                    printf("nickname existed\n");
                    user->wbuf = PUT_MSG(C_account_already_exist);
                    return CLT_CAN_WRITE;
                }
                else{
                    printf("unknown sign up fail\n");
                    exit(1);
                } 
            }
            else {
                printf("sign up success\n");
                login_user.insert(user->name);
                user->state = US_IN_LOBBY;
                user->wbuf = PUT_MSG(C_login_success);
                return CLT_CAN_WRITE;
            }
        }
        else if(cmd_id == C_login_to_server)
        {
            if(login_user.find(user->name) != login_user.end()){
                printf("user: %s has already login\n", user->name.c_str());
                user->wbuf = PUT_MSG(C_already_login);
            }
            if(!Login(db_handler, ss, user)){
                printf("login fail\n");
            }
            else {
                printf("login success\n");
                user->state = US_IN_LOBBY;
                user->wbuf = PUT_MSG(C_login_success);
                return CLT_CAN_WRITE;
            }
        }
        cmd_q.pop();
    }
    return 1;
}

bool Login(db_conn *db_handler, stringstream &ss, User *user)
{
    string username, passwd_hash;
    ss >> username >> passwd_hash;
    user->name = username;
    db_get_hash(db_handler, username.data(), passwd_hash.data());
    db_handler->res_info->passwd_hash[64] = 0;
    string legit_code = db_handler->res_info->passwd_hash;
    if(legit_code!=passwd_hash){
        
    }
    cout << legit_code << "\n";
    return (legit_code==passwd_hash);
}

unsigned int SignUp(db_conn *db_handler, stringstream &ss, User *user)
{
    string username, passwd_hash;
    ss >> username >> passwd_hash;
    user->name = username;
    return (db_add_user(
        db_handler, 
        username.data(), 
        passwd_hash.data()
    ));
}
