#include "../lib/universal.h"
#include "../lib/command.h"

#include "objects/objects.h"
#include "io/io.h"
#include "dispatcher/dispatcher.h"
#include "logic/logic.h"
#include "parser/parser.h"
#include "utils/utils.h"
#include "mariadb_api/mariadb_api.h"
#include "db_worker/db_worker.h"

#define MAX_JOB 10
#define MAX_EVENT 50

#define evfd events[i].data.fd
#define evtype events[i].events

typedef enum{
    RES_SIGNUP_SUCCESS,
    RES_SIGNUP_DUPNAME,
    RES_LOGIN_SUCCESS,
    RES_LOGIN_USERDNE,
    RES_LOGIN_PWDFAIL,
    DB_LOGIN,
    DB_SIGNUP,
} Action_t;