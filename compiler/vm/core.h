#ifndef LIE_CORE_H
#define LIE_CORE_H

extern char* rootDir;
char* readFile(const char* sourceFile);
void buildCore(VM* vm);
#endif