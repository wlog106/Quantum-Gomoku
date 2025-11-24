#include "mariadb_api.h"
#include <mariadb/mysql.h>
#include <stdlib.h>

db_conn *db_init()
{
    db_conn *db_handler = (db_conn *)malloc(sizeof(db_conn));
    /* initialize connection information */
    const char *host    = getenv("DB_HOST");
    const char *user    = getenv("DB_ADMIN");
    const char *passwd  = getenv("DB_PASSWD");
    const char *db      = getenv("DB_NAME");

    /* initialize connection handler */
    db_handler->conn = mysql_init(NULL);
    Mysql_real_connect(db_handler->conn, host, user, passwd, db);

    /* initialize statement handlers */
    const char *sql_add_user = "SELECT passwd FROM users WHERE name = ?;";
    const char *sql_get_hash = "INSERT INTO users (name, passwd) VALUES (?, ?);";
    db_handler->stmt_add_user = Mysql_stmt_init(db_handler->conn);
    db_handler->stmt_get_hash = Mysql_stmt_init(db_handler->conn);
    Mysql_stmt_prepare(
        db_handler->stmt_add_user, 
        sql_add_user, 
        strlen(sql_add_user)
    );
    Mysql_stmt_prepare(
        db_handler->stmt_get_hash,
        sql_get_hash, 
        strlen(sql_get_hash)
    );

    /* bind some results */
    MYSQL_BIND bind_result[1];
    memset(bind_result, 0, sizeof(MYSQL_BIND));
    db_handler->res_info = (db_response *)malloc(sizeof(db_response));
    bind_result[0].buffer_type     = MYSQL_TYPE_STRING;
    bind_result[0].buffer          = db_handler->res_info->passwd_hash;
    bind_result[0].buffer_length   = 65;
    bind_result[0].length          = NULL;

    return db_handler;
}

unsigned int db_add_user(
    db_conn *db_handler, 
    char *username, 
    char *passwd_hash
){
    unsigned int error = 0;
    MYSQL_BIND bind_param[2];
    memset(bind_param, 0, 2*sizeof(MYSQL_BIND));
    bind_param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
    bind_param[0].buffer      = username;
    bind_param[1].buffer_type = MYSQL_TYPE_STRING;
    bind_param[1].buffer      = passwd_hash;
    error |= Mysql_stmt_bind_param(db_handler->stmt_add_user, bind_param);
    error |= Mysql_stmt_execute(db_handler->stmt_add_user);
    return error;
}

unsigned int db_get_hash(
    db_conn *db_handler, 
    char *username, 
    char *passwd_hash
){
    unsigned int error = 0;
    MYSQL_BIND bind_param[1];
    memset(bind_param, 0, sizeof(MYSQL_BIND));
    bind_param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
    bind_param[0].buffer      = username;
    error |= Mysql_stmt_bind_param(db_handler->stmt_get_hash, bind_param);
    error |= Mysql_stmt_execute(db_handler->stmt_get_hash);
    mysql_stmt_fetch(db_handler->stmt_get_hash);
    return error;
}