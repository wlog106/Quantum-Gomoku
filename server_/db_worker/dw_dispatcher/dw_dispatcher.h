#ifndef DW_DISPATCHER_H
#define DW_DISPATCHER_H

struct Dw_response_t;
struct conn_t;
struct DwContext;

void dw_dispatcher(
    DwContext *dwcxt
);

void dw_signup(DwContext *dwcxt);

void dw_login(DwContext *dwcxt);

#endif