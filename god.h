#pragma once
#include "manager.h"
#include <cstddef>
#include <new>

class BaseMemoryManager : public IMemoryManager{
public:
    void* Alloc(size_t size) override { return malloc(size); }

    void Free(void* ptr) override { free(ptr); }
};

class CMemoryGod {
public:
    static IMemoryManager* current_alloc;
    static BaseMemoryManager base;
};


void* operator new(size_t size);
void* operator new(size_t size, const std::nothrow_t&) noexcept;
void* operator new[](size_t size);
void* operator new[](size_t size, const std::nothrow_t&) noexcept;

void operator delete(void* ptr) noexcept ;
void operator delete(void* ptr, const std::nothrow_t&) noexcept;
void operator delete[](void* ptr) noexcept ;
void operator delete[](void* ptr, const std::nothrow_t&) noexcept;
