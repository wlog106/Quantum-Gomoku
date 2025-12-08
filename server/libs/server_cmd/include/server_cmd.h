#ifndef SERVER_CMD_H
#define SERVER_CMD_H

typedef enum{
    UNKNOWN_USR,
    LOGGEDIN_USR,
} userState;

typedef enum{
    DW_SIGNUP,
    DW_LOGIN,
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