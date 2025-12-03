#ifndef DW_DISPATCHER_H
#define DW_DISPATCHER_H

struct Dw_response_t;
struct conn_t;
struct DwContext;

void dw_dispatcher(
    int n, 
    struct iovec *iov,
    DwContext *dwcxt
);

void dw_signup(DwContext *dwcxt, conn_t *u);
void dw_login(DwContext *dwcxt, conn_t *u);

#endif