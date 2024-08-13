#pragma once

#include <pthread.h>
#include <atomic>
#include "shared_memory.hpp"

class NamedMutex {
public:
    NamedMutex(const char* name);
    ~NamedMutex();

    void lock();
    void unlock();
    bool try_lock();

    static bool remove(const char* name);

private:
    struct Handle {
        std::atomic_flag flag;
        int state;
        pthread_mutex_t mutex;
    };

    SharedMemory<Handle> handle_; 
};
