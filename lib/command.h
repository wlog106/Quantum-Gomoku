typedef enum {
    C_create_new_account,
    C_login_to_server,
    C_client_logout,
    C_create_room,
    C_join_room_by_id,
    C_pair_randomly,
    C_observe_randomly,
    /*above is client->server, below is server->client*/
    //login
    C_account_does_not_exist,
    C_account_already_exist,
    C_password_incorrect,
    C_login_success,
    C_already_login,
    
    //"room info" : "room_id p1_exist p2_exist ob1_exist ob2_exist ob3_exist p1_ready p2_ready {user name by order}"
    /*
    p1:andy(ready)
    p2:NULL
    ob1:judy
    ob2:NULL
    ob3:david

    "CF4DS 1 0 1 0 1 1 0 andy judy david"
    */
    //opselect
    //create room
    C_create_room_success, //cid "room info"
    C_too_much_room, //cid

    //join room by id
    C_join_by_id_success_waiting,//cid "room info"
    C_join_by_id_success_playing,//to be decided
    C_join_by_id_fail, //cid "reason" (1:id DNE, 2:room full)

    //pair randomly
    C_pair_success_start_waiting,//cid "room info"
    C_pair_fail,//cid 

    //observe randomly
    C_start_observing,
    C_no_current_playing_room

} Command_t;

/*
    command format>
        C_create_new_accout:    100:int username:str passwd_hash:str
        C_login_to_server:      101:int username:str passwd_hash:str 
*/