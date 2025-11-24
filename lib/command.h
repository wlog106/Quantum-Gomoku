#define C_create_new_account 100
#define C_login_to_server 101
#define C_account_does_not_exist 102
#define C_account_already_exist 103
#define C_password_incorrect 104
#define C_login_success 105


/*
    command format>
        C_create_new_accout:    100:int  username:str passwd_hash:str
        C_login_to_server:      101:int  username:str passwd_hash:str 
*/