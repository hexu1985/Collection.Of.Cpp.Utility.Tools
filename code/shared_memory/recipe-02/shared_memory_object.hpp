#pragma once

#include <stddef.h>

class SharedMemoryObject {
public:
    SharedMemoryObject() noexcept;
    SharedMemoryObject(const char* name, bool check_exists=false);
    ~SharedMemoryObject();

    SharedMemoryObject(SharedMemoryObject&& other);
    SharedMemoryObject& operator= (SharedMemoryObject&& other);

    void truncate(size_t length);
    size_t size() const;
    int fileno() const;

    void swap(SharedMemoryObject& other) noexcept;

    static bool remove(const char* name) noexcept;

private:
    SharedMemoryObject(const SharedMemoryObject&) = delete;
    SharedMemoryObject& operator= (const SharedMemoryObject&) = delete;

private:
    int fd_ = -1;
};  
