#pragma once

#include <stddef.h>

class SharedMemory {
public:
    SharedMemory() noexcept;
    ~SharedMemory();

    SharedMemory(SharedMemory&& other);
    SharedMemory& operator= (SharedMemory&& other);

    void truncate(size_t length);
    size_t size() const;
    int fileno() const;

    void* map(size_t length, bool readonly=false, long offset=0);

    void swap(SharedMemory& other) noexcept;

    static bool exists(const char* name) noexcept;
    static bool remove(const char* name) noexcept;
    static void unmap(void* addr, size_t length) noexcept;

    static SharedMemory create_only(const char* name);
    static SharedMemory open_or_create(const char* name);
    static SharedMemory open_read_write(const char* name);
    static SharedMemory open_read_only(const char* name);

private:
    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator= (const SharedMemory&) = delete;

    explicit SharedMemory(int fd);

private:
    int fd_ = -1;
};  
