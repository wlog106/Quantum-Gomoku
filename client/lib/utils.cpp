#include "client.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::queue;

int Login(int sockfd){
    string user_input;
    cout << "Create new account?[Y/n]" << endl;
    cin >> user_input;
    string account, password, password_confirm;
    char sendline[MAXLINE] = {};
    if(user_input == "y" || user_input == "Y"){
        cout << "Enter account:";
        cin >> account;
        while(true){
            cout << "Enter password:";
            cin >> password;
            cout << "Enter password again:";
            cin >> password_confirm;
            if(password == password_confirm){
                break;
            }
            else{
                cout << "Password does not match.\n";
            }
        }
        sprintf(sendline, "%d %s %s\n", C_create_new_account, account, password);
    }
    else{
        cout << "Enter account:";
        cin >> account;
        cout << "Enter password:";
        cin >> password;
        sprintf(sendline, "%d %s %s\n", C_login_to_server, account, password);
    }
    Write(sockfd, sendline, sizeof(sendline));


    Read_commamd(sockfd,);//receive ack
    return;//return if login success
}