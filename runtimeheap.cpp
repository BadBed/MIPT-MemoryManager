#include "runtimeheap.h"

void* CRuntimeHeap::Alloc (size_t size) {
    return malloc(size);
}

void CRuntimeHeap::Free (void* ptr) {
    free(ptr);
}
