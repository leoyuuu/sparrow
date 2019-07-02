#ifndef LIE_OBJ_THREAD_H
#define LIE_OBJ_THREAD_H
#include "obj_fn.h"

// 这里是用户态线程，相当于 go groutine. 一种简单实现
typedef struct objThread {
    ObjHeader objHeader;

    Value* stack; // 运行时栈顶
    Value* esp;  // 运行时栈底
    uint32_t stackCapacity; // 栈容量

    Frame* frames; // 调用框架
    uint32_t usedFrameNum; 
    uint32_t frameCapacity;

    ObjUpvalue* openUpvalues;

    // 当前 thread 的调用者
    struct objThread* caller;

    // 运行时错误的对象放在这里。默认为空
    Value errorObj;
} ObjThread;



void prepareFrame(ObjThread* objThread, ObjClosure* objCloosure, Value* stackStart);
ObjThread* newObjThread(VM* vm, ObjClosure* objClosure);
void resetThread(ObjThread* objThread, ObjClosure* objClosure);

#endif