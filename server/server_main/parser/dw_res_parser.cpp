#include <server_objects.h>
#include <server_utils.h>

#include "../logic/logic.h"

void dw_res_parser(
    ServerContext *scxt,
    ServerObjects *sobj
){  
    char *line;
    int id, fd;
    dw_res newDwres;
    std::queue<dw_res> q;
    /*
        database worker response format:
        > "fd id type:result"
        dw_res_parser decode it and give
        dw_res_processor the following info:
        > conn *u, int type, int result
    */
    while((
        line = sobj->dwr_buf->getline())!=NULL
    ){
        sscanf(line, "%d %d %d:%d",
               &fd, &id, &newDwres.type, &newDwres.result);
        newDwres.u = get_user(fd, sobj->fd_to_conn);
        newDwres.u->id = id;
        newDwres.u->db_job_pending = false;
        q.push(newDwres);
        free(line);
    }
    dw_res_processor(scxt, q);
}