#pragma once

#include "interprocess_semaphore.hpp"
#include "shared_memory.hpp"
#include "interprocess_once.hpp"

class NamedSemaphore {
public:
    NamedSemaphore() noexcept;
    ~NamedSemaphore();

    NamedSemaphore(NamedSemaphore&& other); 
    NamedSemaphore& operator= (NamedSemaphore&& other); 

    void post();
    void wait();
    bool try_wait();
    int get_value();

    static bool exists(const char* name) noexcept;
    static bool remove(const char* name) noexcept;

    static NamedSemaphore create_only(const char* name, unsigned int value);
    static NamedSemaphore open_or_create(const char* name, unsigned int value);
    static NamedSemaphore open_only(const char* name);

private:
    NamedSemaphore(const NamedSemaphore&) = delete;
    NamedSemaphore& operator= (const NamedSemaphore&) = delete;

    struct Impl {
        InterprocessOnceFlag once_flag;
        InterprocessSemaphore semaphore;
    };

    explicit NamedSemaphore(SharedMemory<Impl>&& impl);
    NamedSemaphore(SharedMemory<Impl>&& impl, unsigned int value);

private:
    SharedMemory<Impl> impl_;
};
