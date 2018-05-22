#include <iostream>
#include <cstddef>
#include "memuniverse.h"
#include "stack_allocator.h"
#include "runtimeheap.h"

class CConservator : public CAllocatedOn<CRuntimeHeap>{
    double d;
};

int* f(IMemoryManager* alloc) {
    CMemoryManagerSwitcher switcher(alloc);
    auto result = new int;
    return result;
}

CConservator* h(IMemoryManager* alloc) {
    CMemoryManagerSwitcher switcher(alloc);
    auto result = new CConservator;
    return result;
}

int main() {
    StackAllocator<int> saloc;
    auto x = new int;
    auto u = f(&saloc);
    auto v = h(&saloc);
    delete x;
    delete u;
    delete v;
}
