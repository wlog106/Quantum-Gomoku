#ifndef DW_PROCESSOR_H
#define DW_PROCESSOR_H

#include <queue>

struct DwContext;

void dw_processor(
    DwContext *dwcxt,
    std::queue<std::pair<int, char*>> &q
);

#endif