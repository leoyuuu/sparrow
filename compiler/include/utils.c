#include "utils.h"
#include "vm.h"
#include "parser.h"
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

void symbolTableClear(VM* vm, SymbolTable* buffer) {
    uint32_t idx = 0;
    while(idx < buffer->count) {
        memManager(vm, buffer->datas[idx++].str, 0, 0);
    }
    StringBufferClear(vm, buffer);
}

void errorReport(void* parser, ErrorType errorType, const char* fmt, ...) {
    char buffer[DEFAULT_BUFFER_SIZE] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, DEFAULT_BUFFER_SIZE, fmt, ap);
    va_end(ap);

    switch(errorType) {
        case ERROR_IO:
        case ERROR_MEM:
            fprintf(stderr, "%s:%d In function %s():%s\n", __FILE__, __LINE__, __func__, buffer);
            break;
        case ERROR_LEX:
        case ERROR_COMPILE:
            ASSERT(parser != NULL, "Parser is null!");
            fprintf(stderr, "%s:%d \"%s\"\n", ((Parser*)parser)->file, ((Parser*)parser)->preToken.lineNo, buffer);
            break;
        case ERROR_RUNTIME:
            fprintf(stderr, "%s\n", buffer);
            break;
        default:
            NOT_REACHED();
    }
    exit(1);
}