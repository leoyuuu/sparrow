#include <stdio.h>
#include "tool.h"
#include "../configs/config.h"

enum ErrTag {
    ERR=1,
    WARN,
    INFO
};

void _log(enum ErrTag tag, char* msg)
{
    if (tag == ERR) 
    {
        fprintf(stderr, "%s", msg);
    }
    else
    {
        fprintf(stdout, "%s", msg);
    }
}

void loge(char* msg)
{
    _log(ERR, msg);
}

void logw(char* msg) 
{
    _log(WARN, msg);
}

void logi(char* msg)
{
    _log(INFO, msg);
}