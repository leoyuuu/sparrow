#ifndef LIE_VM_H
#define LIE_VM_H

#include "common.h"

struct vm {
    uint32_t allocatedBytes;
    Parser* curParser;
};

void initVM(VM* vm);

VM* newVM(void);

#endif