#pragma once

#include "interprocess_mutex.hpp"
#include "interprocess_condition.hpp"

class InterprocessSemaphore {
public:
    InterprocessSemaphore(unsigned int value);
    ~InterprocessSemaphore();

    void post();
    void wait();
    bool try_wait();
    int get_value();

private:
    InterprocessMutex mutex_;
    InterprocessCondition cond_;
    unsigned int count_;
};
