/**
 * @file spin_lock.hpp
 * @brief 一个自旋锁类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-06
 *
 * @see C++ Concurrency in Action, Chapter 5, spinlock_mutex \n
 * https://github.com/subjam/concurrency-in-action
 */
#ifndef MINI_UTILS_SPIN_LOCK_INC
#define MINI_UTILS_SPIN_LOCK_INC

#include <atomic> 

namespace MiniUtils {

/**
 * @brief 自旋锁
 */
class SpinLock {
    std::atomic_flag flag_;

public:
    /**
     * @brief 创建一个自旋锁
     */
    SpinLock(): flag_(ATOMIC_FLAG_INIT) {}

    /**
     * @brief 获取锁
     */
    void lock() 
    {
        while (flag_.test_and_set(std::memory_order_acquire));
    }

    /**
     * @brief 释放锁
     */
    void unlock()
    {
        flag_.clear(std::memory_order_release);
    }
};

}   // namespace MiniUtils

#endif
