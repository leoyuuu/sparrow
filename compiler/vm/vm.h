#ifndef LIE_VM_H
#define LIE_VM_H

#include "common.h"
#include "header_obj.h"
#include "meta_obj.h"


typedef enum vmResult {
    VM_RESULT_SUCCESS,
    VM_RESULT_ERROR
} VMResult;

struct vm {
    Class* classOfClass;
    Class* objectClass;
    Class* stringClass;
    Class* mapClass;
    Class* listClass;
    Class* rangeClass;
    Class* nullClass;
    Class* boolClass;
    Class* numClass;
    Class* fnClass;
    uint32_t allocatedBytes;
    ObjHeader* allObjects;
    Parser* curParser;
};

void initVM(VM* vm);

VM* newVM(void);

#endif