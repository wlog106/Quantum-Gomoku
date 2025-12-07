#pragma once
typedef enum {
    //login
    C_create_new_account,//cid "account" "password hash"
    /*reply
        C_account_already_exist
        C_login_success
    */
    C_login_to_server,//cid "account" "password hash"
    /*reply
        C_account_does_not_exist
        C_password_incorrect
        C_already_login
        C_login_success
    */
    C_client_logout,//cid //no reply


    //opselect
    C_create_room,//cid
    /*reply
        C_create_room_success
        C_too_much_room
    */
    C_join_room_by_id,//cid "room id"
    /*reply
        C_join_by_id_success_waiting
        C_join_by_id_success_playing
        C_join_by_id_fail
        C_start_a_playing_room
    */
    C_pair_randomly,//cid
    /*reply
        C_pair_success_start_waiting
        C_pair_fail
    */
    C_observe_randomly,//cid
    /*reply
        C_start_observing
        C_no_current_playing_room
        C_start_a_playing_room
    */


    //waiting room
    C_change_ready,//cid
    /*reply
        C_new_room_info
        check if all members are ready, if yes, start the game
        C_start_a_playing_room
    */
    C_change_waiting_position,//cid "position[1,5]"
    /*reply
        C_new_room_info
    */
    C_leave_waiting_room,//cid
    /*reply
        C_leave_waiting_room_success
        c_new_room_info
    */
    C_send_message_waiting_room,//cid "message"
    /*reply
        C_new_waiting_room_message
    */
    
    //playing
    C_playing_move_finish, //cid piece_pos_x piece_pos_y piece_type player1_time player2_time
    C_want_to_observe,

    //------------------------------------------------//
    /*above is client->server, below is server->client*/
    //------------------------------------------------//

    //login
    C_account_does_not_exist,//cid
    C_account_already_exist,//cid
    C_password_incorrect,//cid
    C_login_success,//cid
    C_already_login,//cid
    
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
    C_start_observing, //to be dicided
    C_no_current_playing_room, // cid

    //waiting room
    C_new_room_info, //cid "room info"
    C_new_waiting_room_message, //cid "message"
    C_leave_waiting_room_success, //cid

    //playing initialize
    /*
    "full game info" : "room_id player_info board_info playing_position"
    > player_info : "player_existance player_names"
     - player_existance : "[0,1] * 5" //each position have user or not
     - player_names : all player names split with space
    > board_info : 169 "piece" with 168 space in row-major
     - piece : "0" or "1" or "3" or "7" or "9"
    > playing_position : [1,5] //who you are
    */
    /*
    "segment info" : "new_pieces time moving_position"
    "new_pieces" : "has_new_piece piece_positon piece_type" //if has_new_piece = 0, don't need to send positon and type
    "has_new_piece" : [0, 1] //0 mean has no new piece(initialize)
    "piece_position" : "x y" [0:Board_size)
    "time" : "player1_time player2_time" //unit: 100ms
    "moving_position" : [1,2] //who is current playing
    */
    C_start_a_playing_room, //cid "full game info"
    C_playing_new_segement, //cid "segment info"
    

} Command_t;