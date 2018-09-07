#include "utils.h"
#include "../vm/vm.h"
#include <stdlib.h>
#include <stdarg.h>

void* memManager(VM* vm, void* ptr, uint32_t oldSize, uint32_t newSize) {
    vm->allocatedBytes += newSize - oldSize;
    if(newSize == 0) {
        free(ptr);
        return NULL;
    }

    return realloc(ptr, newSize);
}

// 找出大于等于 v 最近的 2 次幂
u_int32_t ceilToPowerOf2(uint32_t v) {
    v += (v==0);
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

DEFINE_BUFFER_METHOD(String)
DEFINE_BUFFER_METHOD(Byte)
DEFINE_BUFFER_METHOD(Char)
DEFINE_BUFFER_METHOD(Int)