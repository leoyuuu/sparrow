#include "parser.h"
#include "core.h"
#include <string.h>
#include "compiler.h"
#include "class.h"
#if DEBUG
#include "debug.h"
#endif

struct compileUnit {
    // 所编译的函数
    ObjFn* fn;

    // 作用域中允许的局部变量
    LocalVar localVars[MAX_LOCAL_VAR_NUM];

    //已分配的局部变量
    uint32_t localVarNum;

    // 记录本层函数所引用的 upvalue
    Upvalue upvalues[MAX_UPVALUE_NUM];

    // 当前正在编译的代码所处的作用域
    int scopeDepth;

    // 当前所使用的 slot 个数
    uint32_t stackSlotNum;

    // 当前正在编译的循环层
    Loop* curLoop;

    // 当前正在编译的类的编译信息
    ClassBookKeep* enclosingClassBk;

    // 包含此编译单元的编译单元，即直接外层
    struct compileUnit* enclosingUnit;

    // 当前Parser
    Parser* parser;
}; // 编译单元

// 在模块 objModule 中定义名为 name, 值为 value 的模块变量
int defineModuleVar(VM* vm, ObjModule* objModule, const char* name, uint32_t length, Value value) {
    if (length > MAX_ID_LEN) {
        // name 指向的变量名可能不以 \0 结束
        char id[MAX_ID_LEN] = {'\0'};
        memcpy(id, name, length);

        // 本函数可能是在编译源码文件之前调用的，
        // 需要区分场景报错
        if (vm->curParser != NULL) {
            COMPILE_ERROR(vm->curParser, "length of identifier \"%s\" should be no more than %d", id, MAX_ID_LEN);
        } else {
            // 编译源码前调用，比如加载核心代码模块时会调用本函数
            MEM_ERROR("length of identifier \"%s\" should be no more than %d", id, MAX_ID_LEN);
        }   
    }

    // 从模块变量名中查找变量，若不存在就添加
    int symbolIndex = getIndexFromSymbolTable(&objModule->moduleVarName, name, length);
    if (symbolIndex < 0) {
        // 添加变量名
        symbolIndex = addSymbol(vm, &objModule->moduleVarName, name, length);
        // 添加变量值
        ValueBufferAdd(vm, &objModule->moduleVarValue, value);
    } else if (VALUE_IS_NUM(objModule->moduleVarValue.datas[symbolIndex])) {
        // 若遇到之前预先生命的模块变量的定义，在此为其赋予正确的值
        objModule->moduleVarValue.datas[symbolIndex] = value;
    } else {
        symbolIndex = -1; // 判断重定义
    }
    
    return symbolIndex;
}