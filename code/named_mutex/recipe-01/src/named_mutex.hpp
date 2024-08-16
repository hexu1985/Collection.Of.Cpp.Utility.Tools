#pragma once

#include <pthread.h>
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
        pthread_mutex_t mutex;
    };

    SharedMemory<Handle> handle_; 
};
