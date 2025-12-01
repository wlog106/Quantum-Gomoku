#ifndef DW_DISPATCHER_H
#define DW_DISPATCHER_H

struct DB_result_t;
struct DwContext;

void dw_dispatcher(
    int n, 
    struct iovec *iov,
    DwContext *dwcxt
);

DB_result_t dw_signup();
DB_result_t dw_login();

#endif