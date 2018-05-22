#include "god.h"
#include <algorithm>
#include "smallfunctions.h"

BaseMemoryManager CMemoryGod::base = BaseMemoryManager();
IMemoryManager* CMemoryGod::current_alloc =
        static_cast<IMemoryManager*>(&CMemoryGod::base);
constexpr int EXTRA_ALLOC_SIZE = alignof(max_align_t);


void* alloc_with_god(size_t size) {
    auto ptr = CMemoryGod::current_alloc->Alloc(size + EXTRA_ALLOC_SIZE);
    *static_cast<IMemoryManager**>(ptr) = CMemoryGod::current_alloc;
    return increase_ptr(ptr, EXTRA_ALLOC_SIZE);
}

void destroy_with_god(void* ptr) noexcept {
    ptr = increase_ptr(ptr, -EXTRA_ALLOC_SIZE);
    (*static_cast<IMemoryManager**>(ptr))->Free(ptr);
}


void* operator new(size_t size) {
    return alloc_with_god(size);
}

void operator delete(void* ptr) noexcept {
    destroy_with_god(ptr);
}


void* operator new(size_t size, const std::nothrow_t&) noexcept {
    try {
        alloc_with_god(size);
    }
    catch (std::bad_alloc&) {
        return nullptr;
    }
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    destroy_with_god(ptr);
}


void* operator new[](size_t size) {
    return alloc_with_god(size);
}

void operator delete[](void* ptr) noexcept {
    destroy_with_god(ptr);
}


void* operator new[](size_t size, const std::nothrow_t&) noexcept {
    try {
        alloc_with_god(size);
    }
    catch (std::bad_alloc&) {
        return nullptr;
    }
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    destroy_with_god(ptr);
}
