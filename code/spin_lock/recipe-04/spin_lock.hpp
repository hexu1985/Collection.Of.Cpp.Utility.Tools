#pragma once

#include <atomic> 
#include <thread>

static_assert(ATOMIC_BOOL_LOCK_FREE == 2, "atomic bool need lock free");

class SpinLock {
private:
    std::atomic_bool flag;

public:
    SpinLock(): flag{false} {}

    void lock() {
        while (flag.exchange(true, std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        flag.store(false, std::memory_order_release);
    }

    bool try_lock() {
        return !flag.exchange(true, std::memory_order_acquire);
    }
};

