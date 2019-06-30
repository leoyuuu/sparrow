#ifndef LIE_VM_H
#define LIE_VM_H

#include "common.h"
#include "header_obj.h"
#include "meta_obj.h"

struct vm {
    Class* stringClass;
    Class* fnClass;
    uint32_t allocatedBytes;
    ObjHeader* allObjects;
    Parser* curParser;
    Class* listClass;
    Class* rangeClass;
    Class* mapClass;
};

void initVM(VM* vm);

VM* newVM(void);

#endif