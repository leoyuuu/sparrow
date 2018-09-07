#include "vm.h"
#include <stdlib.h>
#include "../include/utils.h"

void initVM(VM* vm) {
    vm->allocatedBytes = 0;
    vm->curParser = NULL;
}

VM* newVM(void) {
    VM* vm = (VM*) malloc(sizeof(VM));
    if(vm == NULL) {
        MEM_ERROR("allocate VM failed!");
    }
    initVM(vm);
    return vm;
}