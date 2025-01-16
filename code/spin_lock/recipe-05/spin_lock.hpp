#pragma once

#include <atomic> 
#include <thread>

class SpinLock {
private:
    std::atomic<int> flag;

public:
    SpinLock(): flag{0} {}

    void lock() {
        int expected = 0;
        while (!flag.compare_exchange_weak(expected, 1)) {
            expected = 0;
        }
    }

    void unlock() {
        flag.store(0);
    }

    bool try_lock() {
        int expected = 0;
        return flag.compare_exchange_weak(expected, 1);
    }
};

