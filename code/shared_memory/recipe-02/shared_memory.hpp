#pragma once

#include "shared_memory_object.hpp"

template<class T>
class SharedMemory {
    SharedMemoryObject object_;
    T* ptr_;

public:
    SharedMemory(const char* name): object_(name) {
        object_.truncate(sizeof(T));
        ptr_ = static_cast<T*>(object_.map(sizeof(T)));
    }

    ~SharedMemory() {
        SharedMemoryObject::unmap(ptr_, sizeof(T));
    }

    T& get() const {
        return *ptr_;
    }
};
