#include "mariadb_api.h"
#include <mariadb/mariadb_com.h>
#include <mariadb/mysql.h>
#include <stdio.h>
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
    const char *sql_add_user = "INSERT INTO users (name, passwd) VALUES (?, ?);";
    const char *sql_get_hash = "SELECT passwd FROM users WHERE name = ?;";
    const char *sql_get_id_by_name = "SELECT id FROM users WHERE name = ?;";
    db_handler->stmt_add_user = Mysql_stmt_init(db_handler->conn);
    db_handler->stmt_get_hash = Mysql_stmt_init(db_handler->conn);
    db_handler->stmt_get_id_by_name = Mysql_stmt_init(db_handler->conn);
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
    db_handler->res_info = (db_response *)malloc(sizeof(db_response));

    /* bind get_hash results */
    MYSQL_BIND bind_get_hash_result[1];
    memset(bind_get_hash_result, 0, sizeof(MYSQL_BIND));
    bind_get_hash_result[0].buffer_type     = MYSQL_TYPE_STRING;
    bind_get_hash_result[0].buffer          = db_handler->res_info->hash;
    bind_get_hash_result[0].buffer_length   = sizeof(db_handler->res_info->hash);
    bind_get_hash_result[0].length          = &db_handler->res_info->pwd_recvlen;
    bind_get_hash_result[0].is_null         = &db_handler->res_info->pwd_is_null;
    Mysql_stmt_bind_result(
        db_handler->stmt_get_hash, 
        bind_get_hash_result
    );

    /* bind get_id_by_name result */
    MYSQL_BIND bind_get_id_by_name_result[1];
    memset(bind_get_id_by_name_result, 0, sizeof(MYSQL_BIND));
    bind_get_id_by_name_result[0].buffer_type   = MYSQL_TYPE_LONG;
    bind_get_id_by_name_result[0].buffer        = &db_handler->res_info->id;
    bind_get_id_by_name_result[0].buffer_length = sizeof(int);
    bind_get_hash_result[0].length              = &db_handler->res_info->id_recvlen;
    bind_get_hash_result[0].is_null             = &db_handler->res_info->id_is_null;
    Mysql_stmt_bind_result(
        db_handler->stmt_get_id_by_name, 
        bind_get_id_by_name_result
    );
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
    bind_param[0].buffer_type   = MYSQL_TYPE_VAR_STRING;
    bind_param[0].buffer        = username;
    bind_param[0].buffer_length = strlen(username);
    bind_param[1].buffer_type   = MYSQL_TYPE_STRING;
    bind_param[1].buffer        = passwd_hash;
    bind_param[1].buffer_length = strlen(passwd_hash);
    error |= Mysql_stmt_bind_param(db_handler->stmt_add_user, bind_param);
    error |= Mysql_stmt_execute(db_handler->stmt_add_user);
    mysql_stmt_free_result(db_handler->stmt_add_user);
    mysql_stmt_reset(db_handler->stmt_add_user);
    error |= Mysql_commit(db_handler->conn);
    return error;
}

unsigned int db_get_id_by_name(
    db_conn *db_handler,
    char *username
){
    unsigned int error = 0;
    MYSQL_BIND bind_param[1];
    memset(bind_param, 0, sizeof(MYSQL_BIND));
    bind_param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
    bind_param[0].buffer      = username;
    bind_param[0].buffer_length = strlen(username);
    error |= Mysql_stmt_bind_param(
        db_handler->stmt_get_id_by_name, 
        bind_param
    );
    error |= Mysql_stmt_execute(db_handler->stmt_get_id_by_name);
    error |= (mysql_stmt_fetch(db_handler->stmt_get_id_by_name) != 0);
    error |= (mysql_stmt_fetch(db_handler->stmt_get_id_by_name) != MYSQL_NO_DATA);
    mysql_stmt_free_result(db_handler->stmt_get_id_by_name);
    mysql_stmt_reset(db_handler->stmt_get_id_by_name);
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
    bind_param[0].buffer_length = strlen(username);
    error |= Mysql_stmt_bind_param(db_handler->stmt_get_hash, bind_param);
    error |= Mysql_stmt_execute(db_handler->stmt_get_hash);
    error |= (mysql_stmt_fetch(db_handler->stmt_get_hash) != 0);
    error |= (mysql_stmt_fetch(db_handler->stmt_get_hash) != MYSQL_NO_DATA);
    mysql_stmt_free_result(db_handler->stmt_get_hash);
    mysql_stmt_reset(db_handler->stmt_get_hash);
    return error;
}