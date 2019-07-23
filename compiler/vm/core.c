#include <string.h>
#include <sys/stat.h>
#include "../include/utils.h"
#include "vm.h"
#include "class.h"
#include "compiler.h"
#include "core.h"

char * rootDir = NULL;

#define CORE_MODULE VT_TO_VALUE(VT_NULL)

//返回值类型是 Value 类型， 且是放在 args[0], args 是 value 数组
// RET_VALUE 的参数就是 Value 类型，无须转换直接赋值
// 它是后面 RET_其它类型的基础
#define RET_VALUE(value)\
    do {\
        args[0] = value;\
        return true;\
    } while(0);\

// 将 obj 转换为 value 后作为返回值
#define RET_OBJ(objPtr) RET_VALUE(OBJ_TO_VALUE(objPtr))

// 将bool 值转换为 Value 后作为返回值
#define RET_BOOL(boolean) RET_VALUE(BOOL_TO_VALUE(boolean))
#define RET_NUM(num) RET_VALUE(NUM_TO_VALUE(num))
#define RET_NULL RET_VALUE(VT_TO_VALUE(VT_NULL))
#define RET_TRUE RET_VALUE(VT_TO_VALUE(VT_TRUE))
#define RET_FALSE RET_VALUE(VT_TO_VALUE(VT_FALSE))

// 设置线程报错
#define SET_ERROR_FALSE(vmPtr, errMsg)\
    do{\
        vmPtr->curThread->errorObj = \
        OBJ_TO_VALUE(newObjString(vmPtr, errMsg, strlen(errMsg)));\
        return false;\
    }while(0);


// 绑定方法 func 到 classPtr 指向的类
#define PRIM_METHOD_BIND(classPtr, methodName, func) {\
    uint32_t length = strlen(methodName);\
    int globalIdx = getIndexFromSymbolTable(&vm->allMethodNames, methodName, length);\
    if(globalIdx == -1){\
        globalIdx = addSymbol(vm, &vm->allMethodNames, methodName, length);\
    }\
    Method method;\
    method.type = MT_PRIMITIVE;\
    method.primFn = func;\
    bindMethod(vm, classPtr, (uint32_t)globalIdx, method);\
}


// 对 object 取反: !object， 结果 false;
static bool primObjectNot(VM* vm UNUSED, Value* args) {
    RET_FALSE;
}

// args[0] == args[1]
static bool primObjectEqual(VM* vm UNUSED, Value* args) {
    Value boolValue = BOOL_TO_VALUE(valueIsEqual(args[0], args[1]));
    RET_VALUE(boolValue);
}


// args[0] != args[1]
static bool primObjectNotEqual(VM* vm UNUSED, Value* args) {
    Value boolValue = BOOL_TO_VALUE(!valueIsEqual(args[0], args[1]));
    RET_VALUE(boolValue);
}

// args[0] is args[1]: 类 0 是否为类 1 的子类
static bool primObjectIs(VM* vm UNUSED, Value* args) {
    // args[1] 必须是 class
    if (!VALUE_IS_CLASS(args[1])) {
        RUN_ERROR("argument must be class");
    }

    Class* thisClass = getClassOfObj(vm, args[0]);
    Class* baseClass = (Class*)(args[1].objHeader);

    while (baseClass != NULL)
    {
        if (thisClass == baseClass) {
            RET_TRUE;
        }
        baseClass = baseClass->superClass;
    }
    
    RET_FALSE;
}

static bool primObjectToString(VM* vm UNUSED, Value* args) {
    Class* class = args[0].objHeader->class;
    RET_VALUE(OBJ_TO_VALUE(class->name));
}

static bool primObjectType(VM* vm, Value* args) {
    Class* class = getClassOfObj(vm, args[0]);
    RET_OBJ(class);
}

static bool primClassName(VM* vm UNUSED, Value* args) {
    RET_OBJ(VALUE_TO_CLASS(args[0])->name);
}

static bool primClassSuperType(VM* vm UNUSED, Value* args) {
    Class* class = VALUE_TO_CLASS(args[0]);
    if (class->superClass != NULL) {
        RET_OBJ(class->superClass);
    }
    RET_VALUE(VT_TO_VALUE(VT_NULL));
}

static bool primClassToString(VM* vm UNUSED, Value* args) {
    RET_OBJ(VALUE_TO_CLASS(args[0])->name)
}

static bool primObjectmetaSame(VM* vm UNUSED, Value* args) {
   Value boolValue = BOOL_TO_VALUE(valueIsEqual(args[1], args[2]));
   RET_VALUE(boolValue);
}

char* readFile(const char* path) {
    FILE* file = fopen(path, "r");
    if(file == NULL) {
        IO_ERROR("Couldn't open file \"%s\"", path);
    }
    struct stat fileStat;
    stat(path, &fileStat);
    size_t fileSize = fileStat.st_size;
    printf("file size: %zu", fileSize);
    char* fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        MEM_ERROR("Couldn't allocate memory for reading file \"%s\"", path);
    }
    size_t numRead = fread(fileContent, sizeof(char), fileSize, file);
    if (numRead < fileSize) {
        IO_ERROR("Couldn't read file \"%s\".\n", path);
    }
    fileContent[fileSize] = '\0';
    fclose(file);
    return fileContent;
}

VMResult executeModule(VM* vm, Value moduleName, const char* moduleCode) {
    return VM_RESULT_ERROR;
}

//table中查找符号symbol 找到后返回索引,否则返回-1
int getIndexFromSymbolTable(SymbolTable* table, const char* symbol, uint32_t length) {
   ASSERT(length != 0, "length of symbol is 0!");
   uint32_t index = 0;
   while (index < table->count) {
      if (length == table->datas[index].length &&
	    memcmp(table->datas[index].str, symbol, length) == 0) {
	 return index;
      }
      index++;
   }
   return -1;
}

//往table中添加符号symbol,返回其索引
int addSymbol(VM* vm, SymbolTable* table, const char* symbol, uint32_t length) {
   ASSERT(length != 0, "length of symbol is 0!");
   String string;
   string.str = ALLOCATE_ARRAY(vm, char, length + 1);
   memcpy(string.str, symbol, length);
   string.str[length] = '\0';
   string.length = length;
   StringBufferAdd(vm, table, string);
   return table->count - 1;
}

//定义类
static Class* defineClass(VM* vm, ObjModule* objModule, const char* name) {
   //1先创建类
   Class* class = newRawClass(vm, name, 0);

   //2把类做为普通变量在模块中定义
   defineModuleVar(vm, objModule, name, strlen(name), OBJ_TO_VALUE(class));
   return class;
}

//使class->methods[index]=method
void bindMethod(VM* vm, Class* class, uint32_t index, Method method) {
   if (index >= class->methods.count) {
      Method emptyPad = {MT_NONE, {0}};
      MethodBufferFillWrite(vm, &class->methods, emptyPad, index - class->methods.count + 1); 
   }
   class->methods.datas[index] = method;
}

//绑定基类
void bindSuperClass(VM* vm, Class* subClass, Class* superClass) {
   subClass->superClass = superClass;

   //继承基类属性数
   subClass->fieldNum += superClass->fieldNum;

   //继承基类方法
   uint32_t idx = 0;
   while (idx < superClass->methods.count) {
      bindMethod(vm, subClass, idx, superClass->methods.datas[idx]); 
      idx++;
   }
}

void buildCore(VM* vm) {
    // create core module, set to vm->allModules
    ObjModule* coreModule = newObjModule(vm, NULL);
    mapSet(vm, vm->allModules, CORE_MODULE, OBJ_TO_VALUE(coreModule));

   //创建object类并绑定方法
    vm->objectClass = defineClass(vm, coreModule, "object");
    PRIM_METHOD_BIND(vm->objectClass, "!", primObjectNot);
    PRIM_METHOD_BIND(vm->objectClass, "==(_)", primObjectEqual);
    PRIM_METHOD_BIND(vm->objectClass, "!=(_)", primObjectNotEqual);
    PRIM_METHOD_BIND(vm->objectClass, "is(_)", primObjectIs);
    PRIM_METHOD_BIND(vm->objectClass, "toString", primObjectToString);
    PRIM_METHOD_BIND(vm->objectClass, "type", primObjectType);

    //定义classOfClass类,它是所有meta类的meta类和基类
    vm->classOfClass = defineClass(vm, coreModule, "class");

    //objectClass是任何类的基类 
    bindSuperClass(vm, vm->classOfClass, vm->objectClass);

    PRIM_METHOD_BIND(vm->classOfClass, "name", primClassName);
    PRIM_METHOD_BIND(vm->classOfClass, "superType", primClassSuperType);
    PRIM_METHOD_BIND(vm->classOfClass, "toString", primClassToString);

    //定义object类的元信息类objectMetaclass,它无须挂载到vm
    Class* objectMetaclass = defineClass(vm, coreModule, "objectMeta");
    
    //classOfClass类是所有meta类的meta类和基类
    bindSuperClass(vm, objectMetaclass, vm->classOfClass);

    //类型比较
    PRIM_METHOD_BIND(objectMetaclass, "same(_,_)", primObjectmetaSame);

    //绑定各自的meta类
    vm->objectClass->header.class = objectMetaclass;
    objectMetaclass->header.class = vm->classOfClass;
    vm->classOfClass->header.class = vm->classOfClass; //元信息类回路,meta类终点
}