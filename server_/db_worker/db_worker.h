#ifndef DB_WORKER_H
#define DB_WORKER_H

#include "dw_io/dw_io.h"
#include "dw_dispatcher/dw_dispatcher.h"
#include "dw_objects/dw_objects.h"

#include <queue>

#define MSG(msg) std::to_string(msg) + "\n"

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