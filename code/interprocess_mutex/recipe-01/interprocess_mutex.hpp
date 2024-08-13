#pragma once

#include <pthread.h>

class InterprocessMutex {
public:
    InterprocessMutex(); 
    ~InterprocessMutex(); 

    void lock();
    bool try_lock();
    void unlock();

private:
    InterprocessMutex(const InterprocessMutex&) = delete;
    InterprocessMutex& operator= (const InterprocessMutex&) = delete;

private:
    pthread_mutex_t mtx_;
};
