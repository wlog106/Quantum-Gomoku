#include <mariadb.h>
#include <server_objects.h>

void dw_set_elo(
    DwContext *dwcxt
){
    int id, new_elo;
    job_t *cur_job = &dwcxt->jobq->front();
    sscanf(cur_job->line, "%d %d", &id, &new_elo);
    unsigned int n = db_set_elo_by_id(dwcxt->dc, id, new_elo);
    if(n!=0){
        printf("set elo error\n");
        exit(1);
    }
    dwcxt->jobq->pop();
}