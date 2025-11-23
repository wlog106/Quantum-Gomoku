/*
    TABLE: users>
    attributes:
        name:       string <-> VARCHAR(50) UNIQUE NOT NULL
        passwd:     string <-> CHAR(64) NOT NULL
        ...

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>

typedef struct {
    char passwd_hash[65];
} db_response;

typedef struct{
    MYSQL *conn;
    MYSQL_STMT *stmt_add_user;
    MYSQL_STMT *stmt_get_hash;
    db_response *res_info;
} db_conn;

/*-----------------mariadb_wrap--------------------*/

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