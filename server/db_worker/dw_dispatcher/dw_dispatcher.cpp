#include <server_objects.h>
#include <server_cmd.h>

#include "dw_dispatcher.h"

#include <sys/epoll.h>

void dw_dispatcher(
    DwContext *dwcxt
){
    std::queue<job_t> *jobq_ptr = dwcxt->jobq;
    while(!jobq_ptr->empty()){
        switch(jobq_ptr->front().type)
        {
            case (DW_SIGNUP):
                dw_signup(dwcxt);
                break;
            case (DW_LOGIN):
                dw_login(dwcxt);
                break;
            case (DW_SET_ELO):
                dw_set_elo(dwcxt);
                break;
        }
    }
}