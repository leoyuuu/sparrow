#ifndef LIE_OBJ_LIST_H
#define LIE_OBJ_LIST_H

#include "class.h"
#include "vm.h"

typedef struct {
    ObjHeader objHeader;
    ValueBuffer elements;
} ObjList;

ObjList* newObjList(VM* vm, uint32_t elementNum);
Value removeElement(VM* vm, ObjList* objList, uint32_t index);
void insertElement(VM* vm, ObjList* objList, uint32_t index, Value value);

#endif 