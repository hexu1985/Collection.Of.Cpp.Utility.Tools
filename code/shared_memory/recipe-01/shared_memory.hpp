#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>

class SharedMemory {
public:
    SharedMemory(const char* name, bool readonly=false);
    ~SharedMemory();

    SharedMemory(SharedMemory&& other);
    SharedMemory& operator= (SharedMemory&& other);

    void truncate(off_t length);
    off_t get_size() const;
    void* get_address() const; 
    void clear();

    static SharedMemory create(const char* name, size_t length, bool check_exists=false);
    static bool remove(const char* name) noexcept;

private:
    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator= (const SharedMemory&) = delete;

private:
    SharedMemory(int fd, void* ptr);

private:
    int fd_ = -1;
    void* ptr_ = nullptr;
};
