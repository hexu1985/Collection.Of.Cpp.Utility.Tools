#pragma once

#include <stddef.h>
#include <stdint.h>

class SharedMemoryObject {
public:
    SharedMemoryObject(const char* name, bool readonly=false);
    ~SharedMemoryObject();

    SharedMemoryObject(SharedMemoryObject&& other);
    SharedMemoryObject& operator= (SharedMemoryObject&& other);

    void truncate(size_t length);
    size_t get_size() const;
    void* get_address() const; 
    void clear();

    static SharedMemoryObject create(const char* name, size_t length, bool check_exists=false);
    static bool remove(const char* name) noexcept;

private:
    SharedMemoryObject(const SharedMemoryObject&) = delete;
    SharedMemoryObject& operator= (const SharedMemoryObject&) = delete;

private:
    SharedMemoryObject(int fd, void* ptr);

private:
    int fd_ = -1;
    void* ptr_ = nullptr;
};
