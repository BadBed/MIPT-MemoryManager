#pragma once
#include <cstdlib>

class IMemoryManager {
public:
    virtual ~IMemoryManager() = default;
    virtual void* Alloc(size_t size) = 0;
    virtual void Free(void* ptr) = 0;
};
