#include "obj_fn.h"
#include "class.h"
#include "vm.h"
#include <string.h>

ObjModule* newObjModule(VM* vm, const char* name) {
    ObjModule* module = ALLOCATE(vm, ObjModule);
    if (module == NULL) {
        MEM_ERROR("allocate module failed!");
    }

    initObjHeader(vm, &module->objHeader, OT_MODULE, NULL);
    StringBufferInit(&module->moduleVarName);
    ValueBufferInit(&module->moduleVarValue);
    module->name = NULL;
    if(name != NULL) {
        module->name = newObjString(vm, name, strlen(name));
    }

    return module;
}

ObjInstance* newObjInstance(VM* vm, Class* c) {
    ObjInstance* instance = ALLOCATE_EXTRA(vm, ObjInstance, sizeof(Value) * c->fieldNum);

    initObjHeader(vm, &instance->objHeader, OT_INSTANCE, c);

    uint32_t idx = 0;
    while (idx < c->fieldNum) {
        instance->fields[idx++] = VT_TO_VALUE(VT_NULL);
    }

    return instance;
}