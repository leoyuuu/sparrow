#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "obj_string.h"
#include "vm.h"
#include "utils.h"

// fnv-1a 
uint32_t hashString(char* str, uint32_t length) {
    uint32_t hashCode = 2166136261, idx = 0;
    while(idx < length) {
        hashCode ^= str[idx];
        hashCode *= 16777619;
        idx++;
    }
    return hashCode;
}


void hashObjString(ObjString* objString) {
    CharValue value = objString->value;
    objString->hashCode = hashString(value.start, value.length);
}


ObjString* newObjString(VM* vm, const char* str, uint32_t length) {
    ASSERT(length == 0 || str != NULL, "str length don't match str!");
    ObjString* objString = ALLOCATE_EXTRA(vm, ObjString, length+1);

    if (objString != NULL) {
        initObjHeader(vm, &objString->objHeader, OT_STRING, vm->stringClass);
        objString->value.length = length;
        if (length > 0) {
            memcpy(objString->value.start, str, length);
        }
        objString->value.start[length] = '\0';
        hashObjString(objString);
    } else {
        MEM_ERROR("Allocating ObjString failed! ");
    }
    return objString;
}
