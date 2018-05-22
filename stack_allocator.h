#pragma once
#include <memory>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <list>
#include "smallfunctions.h"
#include "manager.h"

template <typename T>
struct SimpleListNode {
    SimpleListNode<T>* node;
    T value;
};

template <typename T>
class SimpleList {
public:
    SimpleList():_node(nullptr) {}
    ~SimpleList() {
        SimpleListNode<T>* cur_node = _node;
        while (cur_node != nullptr) {
            cur_node = _node->node;
            free(_node);
        }
    }
    void add(T val) {
        auto new_node = static_cast<SimpleListNode<T>*>
                (malloc(sizeof(SimpleListNode<T>)));
        new_node->value = val;
        new_node->node = _node;
        _node = new_node;
    }

    SimpleListNode<T>* _node;
};


class RealAllocator {
public:
	RealAllocator();
	~RealAllocator();

	void* allocate(size_t align, size_t size);
private:
	static constexpr size_t _PAGE_SIZE = 4096;

	void new_page(size_t min_size);
	void* alloc_on_current_page(size_t size);

	size_t _current_full_size;
	size_t _current_free_size;
	void* _current_ptr;
	SimpleList<void*> _pages;
};

template <typename T>
class StackAllocator : public IMemoryManager {
public:
	StackAllocator();
	~StackAllocator() override = default;
	template <typename U>
	StackAllocator(const StackAllocator<U>&);
	template <typename U>
    StackAllocator(StackAllocator<U>&&);

	T* allocate(size_t size);
	void deallocate(T* ptr, size_t size);

	template <typename U>
	void destroy(U* ptr);

	template<typename U, class... Args>
	void construct(U* ptr, Args&&... args);

	template<typename U>
	struct rebind { typedef StackAllocator<U> other; };

	void* Alloc(size_t size) override {
        std::cerr << "Allocated on Stack allocator\n";
	    return _real_allocator->allocate(size,
                std::min(size, alignof(std::max_align_t))); // ?

	}
	void Free(void*) override { std::cerr << "Deallocated on Stack allocator\n"; }

	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	template <typename U>
    friend class StackAllocator;
private:
	std::shared_ptr<RealAllocator> _real_allocator;
};

//******************************************************************

template <typename T>
StackAllocator<T>::StackAllocator() {
    _real_allocator = std::make_shared<RealAllocator>();
}

template <typename T>
template <typename U>
StackAllocator<T>::StackAllocator(const StackAllocator<U>& alloc) {
    _real_allocator = alloc._real_allocator;
}

template <typename T>
template <typename U>
StackAllocator<T>::StackAllocator(StackAllocator<U>&& alloc):
    _real_allocator(std::move(alloc._real_allocator))
{}

template <typename T>
T* StackAllocator<T>::allocate(size_t size) {
    return static_cast<T*>(_real_allocator->allocate(alignof(T), size*sizeof(T)));
}

template <typename T>
void StackAllocator<T>::deallocate(T* ptr, size_t size) {}

template <typename T>
template <typename U>
void StackAllocator<T>::destroy(U *ptr) {
	ptr->~U();
}

template <typename T>
template<typename U, class... Args>
void StackAllocator<T>::construct(U* ptr, Args&&... args ) {
	new((void *)ptr) U(std::forward<Args>(args)...);
};
