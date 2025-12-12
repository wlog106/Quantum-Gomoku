#ifndef SERVER_CMD_H
#define SERVER_CMD_H

typedef enum{
    UNKNOWN_USR,
    LOGGEDIN_USR,
} userState;

typedef enum{
    /* db_worker */
    DW_SIGNUP,
    DW_LOGIN,

    /* opselect */
    CREATE_ROOM,
    JOIN_ROOM_BY_ID,
    PAIR_RANDOMLY,
    OBSERVE_RANDOMLY,

    /* waiting room */
    TOGGLE_READY,
    CHANGE_POS,
    LEAVE_WAITING_ROOM,
    SENDMSG_WAITING_ROOM,

    /* playing room */
    UPDATE_GAME_INFO,
    RUN_OUT_OF_TIME,
    F_SET_OBSERVE,

    /* on writable */
    SEND_OBSERVE_RESULT,
    RES_USR
} jobType;

typedef enum{
    RES_SIGNUP_SUCCESS,
    RES_SIGNUP_DUPNAME,
    RES_LOGIN_SUCCESS,
    RES_LOGIN_USERDNE,
    RES_LOGIN_PWDFAIL,
    DB_LOGIN,
    DB_SIGNUP,
} Action_t;

enum Dw_response_job_t{
    DW_JOB_SIGNUP,
    DW_JOB_LOGIN
};

enum Dw_response_result_t{
    DW_RESULT_SUCCESS,
    /* error result */
    DW_ERESULT_DUPNAME,
    DW_ERESULT_USERDNE,
    DW_ERESULT_PWDFAIL
};

#endif