#pragma once

#include "shared_memory_object.hpp"

template<class T>
class SharedMemory {
    SharedMemoryObject shd_mem_obj_;
    T* ptr_;

public:
    explicit SharedMemory(const char* name): shd_mem_obj_(name) {
        shd_mem_obj_.truncate(sizeof(T));
        ptr_ = static_cast<T*>(shd_mem_obj_.map(sizeof(T)));
    }

    ~SharedMemory() {
        SharedMemoryObject::unmap(ptr_, sizeof(T));
    }

    T& get() const {
        return *ptr_;
    }
};
