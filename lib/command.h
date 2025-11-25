typedef enum {
    C_create_new_account,
    C_login_to_server,
    C_client_logout,
    /*above is client->server, below is server->client*/
    C_account_does_not_exist,
    C_account_already_exist,
    C_password_incorrect,
    C_login_success,
    C_already_login
} Command_t;

/*
    command format>
        C_create_new_accout:    100:int username:str passwd_hash:str
        C_login_to_server:      101:int username:str passwd_hash:str 
*/