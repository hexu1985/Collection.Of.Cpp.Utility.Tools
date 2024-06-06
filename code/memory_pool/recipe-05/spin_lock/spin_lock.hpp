#pragma once

#include <atomic> 
#include <thread>

class SpinLock {
private:
    std::atomic_flag flag;

public:
    SpinLock(): flag{ATOMIC_FLAG_INIT} {}

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }

    bool try_lock() {
        return !flag.test_and_set(std::memory_order_acquire);
    }
};

