#ifndef MEMORYUTIL_H
#define MEMORYUTIL_H

#include <windows.h>

class MemoryUtil
{
public:
    MemoryUtil();

    static HANDLE getProcessHandle();
};

#endif // MEMORYUTIL_H
