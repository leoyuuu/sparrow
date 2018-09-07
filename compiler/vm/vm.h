#ifndef VM_H
#define VM_H
#include "../include/common.h"

struct vm {
    uint32_t allocatedBytes;
    Parser* curParser;
}

void initVM(VM* vm);

VM* newVM(void);

#endif