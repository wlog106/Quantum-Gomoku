#ifndef MARIADB_API_H
#define MARIADB_API_H
/*
    TABLE: users>
    attributes:
        id:         INT AUTO_INCREMENT PRIMARY
        name:       string <-> VARCHAR(50) UNIQUE NOT NULL
        passwd:     string <-> CHAR(64) NOT NULL
        create_at   TIMESTAMP
        ...

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>

typedef struct db_response{
    int id;
    int elo;
    char hash[65];
    unsigned long id_recvlen;
    unsigned long elo_recvlen;
    unsigned long pwd_recvlen;
    my_bool id_is_null;
    my_bool elo_is_null;
    my_bool pwd_is_null;
} db_response;

typedef struct db_conn{
    MYSQL *conn;
    MYSQL_STMT *stmt_add_user;
    MYSQL_STMT *stmt_get_hash;
    MYSQL_STMT *stmt_get_id_by_name;
    MYSQL_STMT *stmt_get_elo_by_name;
    db_response *res_info;
} db_conn;

/*-----------------mariadb_wrap--------------------*/

unsigned int Mysql_commit(MYSQL *conn);

void Mysql_real_connect(
    MYSQL *conn, 
    const char *host, 
    const char *user, 
    const char *passwd, 
    const char *db
);

MYSQL_STMT *Mysql_stmt_init(MYSQL *conn);

void Mysql_stmt_prepare(
    MYSQL_STMT *stmt, 
    const char *query, 
    unsigned long length
);

unsigned int Mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bnd);

void Mysql_stmt_bind_result(MYSQL_STMT *stmt, MYSQL_BIND *bnd);

unsigned int Mysql_stmt_execute(MYSQL_STMT *stmt);

/*-----------------mariadb_util--------------------*/

// wrap C api endpoint with `extern "C"` since C++ code would call it 

#ifdef __cplusplus
extern "C" {
#endif

db_conn *db_init();

/* add new user */
unsigned int db_add_user(
    db_conn *db_handler, 
    char *username, 
    char *passwd_hash
);

/* compare passwd hash base on given username */
unsigned int db_get_hash(
    db_conn *db_handler, 
    char *username, 
    char *passwd_hash
);

unsigned int db_get_id_by_name(
    db_conn *db_handler,
    char *username
);

unsigned int db_get_elo_by_name(
    db_conn *db_handler,
    char *username
);

#ifdef __cplusplus
}
#endif

#endif