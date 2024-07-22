#pragma once

#include "shared_memory.hpp"

template<class T>
class SharedMemoryObject {
    SharedMemory shared_memory_;
    T* ptr_;

public:
    SharedMemoryObject(const char* name): shared_memory_(name) {
        shared_memory_.truncate(sizeof(T));
        ptr_ = static_cast<T*>(shared_memory_.map(sizeof(T)));
    }

    ~SharedMemoryObject() {
        SharedMemory::unmap(ptr_, sizeof(T));
    }

    T& get() const {
        return *ptr_;
    }
};
