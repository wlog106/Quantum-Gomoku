#include "../socket_reader.h"

void login_with_account_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    /*
    switch (cmd_id) {
        
    }*/
}

void creating_account_recv(const string &command){
    stringstream ss(command);
    int cmd_id;
    ss >> cmd_id;
    /*
    switch (cmd_id) {
        
    }*/
}