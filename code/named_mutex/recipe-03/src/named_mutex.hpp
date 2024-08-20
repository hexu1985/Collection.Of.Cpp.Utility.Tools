#pragma once

#include <pthread.h>
#include "shared_memory.hpp"
#include "interprocess_mutex.hpp"
#include "interprocess_once.hpp"

class NamedMutex {
public:
    NamedMutex(const char* name);
    ~NamedMutex();

    void lock();
    void unlock();
    bool try_lock();

    pthread_mutex_t* native_handle(); 

    static bool remove(const char* name);

private:
    struct Impl {
        InterprocessOnceFlag once_flag;
        InterprocessMutex mutex;
    };

    SharedMemory<Impl> impl_; 
};
