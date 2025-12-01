#ifndef DW_IO_H
#define DW_IO_H

struct DwContext;

void on_recv_job(DwContext *dwcxt);
void on_report(DwContext *dwcxt);

#endif