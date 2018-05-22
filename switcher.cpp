#include "switcher.h"

CMemoryManagerSwitcher::CMemoryManagerSwitcher(IMemoryManager* manager) {
    current = manager;
    previous = CMemoryGod::current_alloc;
    CMemoryGod::current_alloc = manager;
}

CMemoryManagerSwitcher::~CMemoryManagerSwitcher() {
    CMemoryGod::current_alloc = previous;
}
