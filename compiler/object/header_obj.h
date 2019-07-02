#ifndef LIE_OBJECT_HEADER
#define LIE_OBJECT_HEADER

#include "utils.h"

typedef enum {
    OT_CLASS,
    OT_LIST,
    OT_MAP,
    OT_MODULE,
    OT_RANGE,
    OT_STRING,
    OT_UPVALUE,
    OT_FUNCTION,
    OT_CLOSURE,
    OT_INSTANCE,
    OT_THREAD
} ObjType;

typedef struct objHeader {
    ObjType type;
    bool isDark; // 是否可达
    Class* class;
    struct objHeader* next;
} ObjHeader;

typedef enum {
    VT_UNDEFINED,
    VT_NULL,
    VT_FALSE,
    VT_TRUE,
    VT_NUM,
    VT_OBJ
} ValueType;


typedef struct {
    ValueType type;
    union {
        double num;
        ObjHeader* objHeader;
    };
} Value;

DECLARE_BUFFER_TYPE(Value)

void initObjHeader(VM* vm, ObjHeader* header, ObjType type, Class* class);

#endif