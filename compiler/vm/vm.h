#ifndef LIE_VM_H
#define LIE_VM_H

#include "common.h"
#include "header_obj.h"
#include "meta_obj.h"
#include "obj_map.h"
#include "obj_thread.h"


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
    Class* threadClass;
    uint32_t allocatedBytes;
    ObjHeader* allObjects;
    SymbolTable allMethoNames;
    ObjMap* allModules;
    ObjThread* curThread;
    Parser* curParser;
};

void initVM(VM* vm);

VM* newVM(void);

#endif