#include <server_objects.h>
#include <share_wrap.h>

void dw_processor(
    DwContext *dwcxt,
    std::queue<std::pair<int, char*>> &q
){
    /* rebuild the job */
    while(!q.empty()){
        job_t newJob;
        char *cmd = (char *)malloc(MAXLINE*sizeof(char));
        newJob.type = q.front().first;
        newJob.line = cmd;
        newJob.r_ptr = cmd;
        newJob.len = strlen(cmd);
        dwcxt->jobq->push(newJob);
    }
}