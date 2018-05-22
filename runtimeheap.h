#pragma once
#include "allocatedon.h"

class CRuntimeHeap {
public:
    static void* Alloc (size_t size);
    static void Free (void* ptr);
};
