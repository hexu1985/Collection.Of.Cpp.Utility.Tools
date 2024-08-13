#pragma once

#include "shared_memory_object.hpp"

template<class T>
class SharedMemory {
    SharedMemoryObject shm_mem_obj_;
    T* ptr_;

public:
    SharedMemory(const char* name): shm_mem_obj_(SharedMemoryObject::open_or_create(name)) {
        shm_mem_obj_.truncate(sizeof(T));
        ptr_ = static_cast<T*>(shm_mem_obj_.map(sizeof(T)));
    }

    ~SharedMemory() {
        SharedMemoryObject::unmap(ptr_, sizeof(T));
    }

    T& get() const {
        return *ptr_;
    }
};
