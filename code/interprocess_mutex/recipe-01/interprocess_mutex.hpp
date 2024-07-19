#pragma once

#include <pthread.h>

class InterprocessMutex {
public:
    static bool create(const char* name);
    static bool remove(const char* name);

    InterprocessMutex(const char* name);
    ~InterprocessMutex();

    void lock();
    void unlock();
    bool try_lock();
};
