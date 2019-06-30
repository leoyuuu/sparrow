#ifndef LIE_OBJ_RANGE_H
#define LIE_OBJ_RANGE_H

#include "class.h"
#include "vm.h"

typedef struct {
    ObjHeader objHeader;
    int from;
    int to;
} ObjRange;

ObjRange* newObjRange(VM* vm, int from, int to);

#endif 