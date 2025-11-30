#include "../server_.h"

job_t::job_t(){}

job_t::job_t(int action, conn_t *user){
    this->action = action;
    this->user = user;
}
