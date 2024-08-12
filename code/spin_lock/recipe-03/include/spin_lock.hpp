#pragma once

#include <atomic> 
#include <thread>

class SpinLock {
private:
    std::atomic_flag flag;

public:
    void lock() {
        // 获取flag之前状态并同时设置flag为设置状态,
        // 如果之前为清除状态, 说明获取锁成功, 否则继续自旋
        while (flag.test_and_set(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        // 设置flag为清除状态
        flag.clear(std::memory_order_release);
    }

    bool try_lock() {
        return !flag.test_and_set(std::memory_order_acquire);
    }
};

