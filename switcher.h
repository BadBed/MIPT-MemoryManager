#pragma once
#include "god.h"
#include "manager.h"

class CMemoryManagerSwitcher {
public:
    explicit CMemoryManagerSwitcher(IMemoryManager*);
    ~CMemoryManagerSwitcher();

private:
    IMemoryManager* current;
    IMemoryManager* previous;
};
