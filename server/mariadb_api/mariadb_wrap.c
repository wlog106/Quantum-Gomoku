#include "mariadb_api.h"
#include <mariadb/mysql.h>
#include <stdint.h>
#include <stdio.h>

unsigned int Mysql_commit(MYSQL *conn)
{
    if(mysql_commit(conn)!=0){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_errno(conn), mysql_error(conn));
        return 1;
    }
    return 0;
}

void Mysql_real_connect(
    MYSQL *conn, 
    const char *host, 
    const char *user, 
    const char *passwd, 
    const char *db
){
    if(!mysql_real_connect(conn, host, user, passwd, db, 0, NULL, 0)){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_errno(conn), mysql_error(conn));
        exit(1);
    }
}

MYSQL_STMT *Mysql_stmt_init(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    if((stmt = mysql_stmt_init(conn)) == 0){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_errno(conn), mysql_error(conn));
        exit(1);
    }
    return stmt;
}

void Mysql_stmt_prepare(
    MYSQL_STMT *stmt, 
    const char *query, 
    unsigned long length
){
    if(mysql_stmt_prepare(stmt, query, length)!=0){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        exit(1);
    }
}

unsigned int Mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bnd)
{
    if(mysql_stmt_bind_param(stmt, bnd)!=0){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return 1;
    }
    return 0;
}

void Mysql_stmt_bind_result(MYSQL_STMT *stmt, MYSQL_BIND *bnd)
{
    if(mysql_stmt_bind_result(stmt, bnd)!=0){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        exit(1);
    }
        //fprintf(stderr, "?Error: code: %u, desc: %s\n",
                //mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
}

unsigned int Mysql_stmt_execute(MYSQL_STMT *stmt)
{
    if(mysql_stmt_execute(stmt)!=0){
        fprintf(stderr, "Error: code: %u, desc: %s\n",
                mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return 1;
    }
    return 0;
}