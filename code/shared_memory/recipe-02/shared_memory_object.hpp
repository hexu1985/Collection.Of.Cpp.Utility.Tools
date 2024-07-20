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

    void* map(size_t length, bool readonly=false, long offset=0);

    void swap(SharedMemoryObject& other) noexcept;

    static bool remove(const char* name) noexcept;
    static void unmap(void* addr, size_t length) noexcept;

private:
    SharedMemoryObject(const SharedMemoryObject&) = delete;
    SharedMemoryObject& operator= (const SharedMemoryObject&) = delete;

private:
    int fd_ = -1;
};  
