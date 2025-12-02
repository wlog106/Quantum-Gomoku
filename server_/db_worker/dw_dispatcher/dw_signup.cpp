#include "../../server_.h"

void dw_signup(DwContext *dwcxt, conn_t *u){
    Dw_response_t dw_res(DW_JOB_SIGNUP);
    unsigned int n = db_add_user(
        dwcxt->dc, 
        u->name.data(), 
        u->hash
    );
    if(n == 1062){
        dw_res.dw_result = DW_ERESULT_DUPNAME;
    }
    if(n != 0){
        /* unknown error */
        exit(1);
    }
    else{
        dw_res.dw_result = DW_RESULT_SUCCESS;
        int e =  db_get_id_by_name(
            dwcxt->dc,
            u->name.data()
        );
        if(e != 0 || dwcxt->dc->res_info->id_is_null){
            /* unknown error */
            exit(1);
        }
        else{
            u->id = dwcxt->dc->res_info->id;
        }
    }
    dwcxt->resultq->push(dw_res);
}