#include "header_obj.h"
#include "class.h"
#include "vm.h"

DEFINE_BUFFER_METHOD(Value)

void initObjHeader(VM* vm, ObjHeader* header, ObjType type, Class* class) {
    header->type = type;
    header->isDark = false;
    header->class = class;
    header->next  = vm->allObjects;
    vm->allObjects = header;
}