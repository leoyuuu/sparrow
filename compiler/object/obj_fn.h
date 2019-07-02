#ifndef LIE_OBJ_FN_H
#define LIE_OBJ_FN_H

#include "utils.h"
#include "meta_obj.h"

typedef struct {
    char* fnName;
    IntBuffer lineNo;
    CharValue value;
} FnDebug;

typedef struct {
    ObjHeader objHeader;
    ByteBuffer instrStream;
    ValueBuffer constants;
    ObjModule* module;

    uint32_t maxStackSlotUsedNum;
    uint32_t upvalueNum;
    uint8_t argNum;
#if DEBUG
    FnDebug* debug;
#endif
} ObjFn;

typedef struct upvalue {
    ObjHeader objHeader;
    Value* localVarPtr;
    Value closedUpvalue;
    struct upvalue* next;
}ObjUpvalue;

typedef struct {
    ObjHeader header;
    ObjFn* fn;
    ObjUpvalue* upvalues[0];
} ObjClosure;

typedef struct {
    uint8_t* ip;
    ObjClosure* closure;
    Value* stackStart;
} Frame;

#define INITIAL_FRAME_NUM 4

ObjUpvalue* newObjUpvalue(VM* vm, Value* localVarPtr);
ObjClosure* newObjClosure(VM* vm, ObjFn* objFn);
ObjFn* newObjFn(VM* vm, ObjModule* module, uint32_t maxStackSlotUsedNum);

#endif