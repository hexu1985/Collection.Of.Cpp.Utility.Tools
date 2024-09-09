#pragma once

#include <utility>
#include "shared_memory_object.hpp"

template <typename T>
class SharedMemory {
public:
    SharedMemory() {
    }

    ~SharedMemory() {
        if (ptr_) {
            SharedMemoryObject::unmap(ptr_, sizeof(T));
        }
    }

    T& get() const {
        return *ptr_;
    }

    SharedMemory(SharedMemory&& other) : shm_mem_obj_(std::move(other.shm_mem_obj_)), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    SharedMemory& operator= (SharedMemory&& other) {
        if (this == &other) {
            return *this;
        }

        if (ptr_) {
            SharedMemoryObject::unmap(ptr_, sizeof(T));
        }

        shm_mem_obj_ = std::move(other.shm_mem_obj_);
        ptr_ = other.ptr_;
        
        other.ptr_ = nullptr;
    }

    static SharedMemory create_only(const char* name) {
        return SharedMemory{SharedMemoryObject::create_only(name)};
    }

    static SharedMemory open_or_create(const char* name) {
        return SharedMemory{SharedMemoryObject::open_or_create(name)};
    }

    static SharedMemory open_read_write(const char* name) {
        return SharedMemory{SharedMemoryObject::open_read_write(name)};
    }

    static SharedMemory open_read_only(const char* name) {
        return SharedMemory{SharedMemoryObject::open_read_only(name)};
    }

    static bool exists(const char* name) noexcept {
        return SharedMemoryObject::exists(name);
    }

    static bool remove(const char* name) noexcept {
        return SharedMemoryObject::remove(name);
    }

private:
    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator= (const SharedMemory&) = delete;

    explicit SharedMemory(SharedMemoryObject&& shm_mem_obj): shm_mem_obj_(std::move(shm_mem_obj)) {
        shm_mem_obj_.truncate(sizeof(T));
        ptr_ = static_cast<T*>(shm_mem_obj_.map(sizeof(T)));
    }

private:
    SharedMemoryObject shm_mem_obj_;
    T* ptr_ = nullptr;
};
