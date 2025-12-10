#include <server_objects.h>

void job_t::fill_line(char *cmd){
    this->line = cmd;
    this->r_ptr = cmd;
    this->len = strlen(cmd);
}