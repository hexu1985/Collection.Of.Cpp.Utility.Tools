#pragma once

#include <stddef.h>

class SharedMemoryObject {
public:
    SharedMemoryObject() noexcept;
    ~SharedMemoryObject();

    SharedMemoryObject(SharedMemoryObject&& other);
    SharedMemoryObject& operator= (SharedMemoryObject&& other);

    void truncate(size_t length);
    size_t size() const;
    int fileno() const;

    void* map(size_t length, bool readonly=false, long offset=0);

    void swap(SharedMemoryObject& other) noexcept;

    static bool exists(const char* name) noexcept;
    static bool remove(const char* name) noexcept;
    static void unmap(void* addr, size_t length) noexcept;

    static SharedMemoryObject create_only(const char* name);
    static SharedMemoryObject open_or_create(const char* name);
    static SharedMemoryObject open_read_write(const char* name);
    static SharedMemoryObject open_read_only(const char* name);

private:
    SharedMemoryObject(const SharedMemoryObject&) = delete;
    SharedMemoryObject& operator= (const SharedMemoryObject&) = delete;

    explicit SharedMemoryObject(int fd);

private:
    int fd_ = -1;
};  
