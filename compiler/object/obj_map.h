#ifndef LIE_OBJ_MAP_H
#define LIE_OBJ_MAP_H

#include "header_obj.h"

#ifndef MAP_LOAD_PERCENT
#define MAP_LOAD_PERCENT 0.8
#endif

#ifndef CAPACITY_GROW_FACTOR
#define CAPACITY_GROW_FACTOR 4
#endif


typedef struct {
    Value key;
    Value value;
} Entry;

typedef struct {
    ObjHeader objHeader;
    uint32_t capacity;
    uint32_t count;
    Entry* entries;
} ObjMap;

ObjMap* newObjMap(VM* vm);
void mapSet(VM* vm, ObjMap* objMap, Value key, Value value);
Value mapGet(ObjMap* objMap, Value key);
void clearMap(VM* vm, ObjMap* objMap);
Value removeKey(VM* vm, ObjMap* objMap, Value key);

#endif 