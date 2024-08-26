#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore(unsigned int value);
    ~Semaphore();

    void post();
    void wait();
    bool try_wait();

private:
    std::mutex mutex_;
    std::condition_variable cond_;
    unsigned int count_;
};
