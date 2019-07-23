#ifndef LIE_COMPILER_H
#define LIE_COMPILER_H
#include "obj_fn.h"

#define MAX_LOCAL_VAR_NUM 128
#define MAX_UPVALUE_NUM 128
#define MAX_ID_LEN 128

#define MAX_METHOD_NAME MAX_ID_LEN
#define MAX_ARG_NUM 16

// 函数名长度 + '(' + N 个参数 + (n-1) 个参数分隔符',' + ')'
#define MAX_SIGN_LEN (MAX_METHOD_NAME_LEN + 2 * MAX_ARG_NUM + 1)

#define MAX_FIELD_NUM 128


typedef struct {
    // 如果此 upvalue 是直接外层函数的局部变量就为 true
    bool isEnclosingLocalVar;

    // 外层函数中局部变量的索引或者外侧鞥函数中 upvalue 的索引
    // 取决于 isEncosingLocalVar 的值
    uint32_t index;
} Upvalue;

typedef struct {
    const char* name;
    uint32_t length;
    int scopeDepth; // 局部变量作用域

    // 表示本函数中的局部变量是否是其内层函数所引用的 upvalue，
    // 当其内层函数引用此变量的时候，由其内层函数设置此项为 true
    bool isUpvalue;
} LocalVar; // 局部变量

typedef enum {
    SIGN_CONSTRUCT, // 构造函数
    SIGN_METHOD, // 普通方法
    SIGN_GETTER,
    SIGN_SETTER,
    SIGN_SUBSCRIPT, // getter 形式的下标
    SIGN_SUBSCRIPT_SETTER, // setter 形式的下标
} SignatureType; // 方法签名类型

typedef struct {
    SignatureType type;
    const char* name; //签名
    uint32_t length; // 签名长度
    uint32_t argNum; // 参数个数
} Signature;

typedef struct loop {
    int condStartIndex; // 循环中条件的地址
    int bodyStartIndex; // 循环体起始地址
    int scopeDepth; // 循环中若有 break, 告诉它需要退出的作用域深度
    int exitIndex; // 循环条件不满足时跳出循环体的目标地址
    struct loop* enclosingLoop; // 外层循环
} Loop;

typedef struct {
    ObjString* name; // 类名
    SymbolTable fields; // 类属性符号表
    bool inStatic; // 当前方法是否为静态方法
    IntBuffer instantMethods; // 实例方法
    IntBuffer staticMethods; // 静态方法
    Signature* signature; // 当前正在编译的签名
} ClassBookKeep;

typedef struct compileUnit CompileUnit;

int defineModuleVar(VM* vm, ObjModule* ObjModule, const char* name, uint32_t length, Value value);


#endif