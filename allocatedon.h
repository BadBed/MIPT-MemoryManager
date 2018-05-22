#pragma once
#include "god.h"

template <class ALLOC>
class CAllocatedOn {
public:
    void* operator new (size_t size);
    void operator delete(void* ptr);
};


template <typename ALLOC>
void* CAllocatedOn<ALLOC>::operator new(size_t size) {
    return ALLOC::Alloc(size);
};

template <typename ALLOC>
void CAllocatedOn<ALLOC>::operator delete(void* ptr) {
    ALLOC::Free(ptr);
}
