#pragma once

#include <pthread.h>
#include <atomic>
#include "shared_memory_object.hpp"

class InterprocessMutex {
public:
    InterprocessMutex(const char* name);
    ~InterprocessMutex();

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

    SharedMemoryObject<Handle> handle_; 
};
