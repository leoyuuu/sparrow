#include "obj_map.h"
#include "class.h"
#include "vm.h"
#include "obj_string.h"
#include "obj_range.h"

ObjMap* newObjMap(VM* vm) {
    ObjMap* objMap = ALLOCATE(vm, ObjMap);
    initObjHeader(vm, &objMap->objHeader, OT_MAP, vm->mapClass);
    objMap->capacity = objMap->count = 0;
    objMap->entries = NULL;
    return objMap;
}

static uint32_t hashNum(double num) {
    Bits64 bits64;
    bits64.num = num;
    return bits64.bits32[0] ^ bits64.bits32[1];
}

static uint32_t hashObj(ObjHeader* objHeader) {
    switch (objHeader->type) {
    case OT_CLASS: {
        return hashString(((Class*)objHeader)->name->value.start, ((Class*)objHeader)->name->value.length);
    }
    case OT_RANGE: {
        ObjRange* rg = (ObjRange*) objHeader;
        return hashNum(rg->from) ^ hashNum(rg->to);
    }
    case OT_STRING: {
        return((ObjString*)objHeader)->hashCode;
    }
    default:
        RUN_ERROR("hash table currently support only string, range, class");
    }
    return 0;
}

static uint32_t hashValue(Value value) {
    switch (value.type)
    {
    case VT_FALSE: return 0;
    case VT_NULL: return 1;
    case VT_TRUE: return 2;
    case VT_NUM: return hashNum(value.num);
    case VT_OBJ: return hashObj(value.objHeader);
    default: RUN_ERROR("unsupported type hash");
    }
    return 0;
}

