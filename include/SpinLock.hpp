/**
 * @file SpinLock.hpp
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
 *
 * @note 基于std::atomic_flag实现
 */
class SpinLock {
private:
    std::atomic_flag flag_;

public:
    /**
     * @brief 创建一个自旋锁, 初始化为清除状态
     */
    SpinLock(): flag_(ATOMIC_FLAG_INIT) {}

    /**
     * @brief 获取锁
     */
    void lock() 
    {
        // 获取flag之前状态并同时设置flag为设置状态,
        // 如果之前为清除状态, 说明获取锁成功, 否则继续自旋
        while (flag_.test_and_set(std::memory_order_acquire));
    }

    /**
     * @brief 释放锁
     */
    void unlock()
    {
        // 设置flag为清除状态
        flag_.clear(std::memory_order_release);
    }

    /**
     * @brief 尝试获取锁
     *
     * @return 获取锁成功返回true, 否则返回false
     */
    bool tryLock()
    {
        return !flag_.test_and_set(std::memory_order_acquire);
    }
};

}   // namespace MiniUtils

#endif
