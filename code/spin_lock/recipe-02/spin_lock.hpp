/**
 * @file spin_lock.hpp
 * @brief 一个自旋锁类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-06
 *
 * @see c++并发编程实战, 第5.2.2章节, spinlock_mutex \n
 * https://github.com/subjam/concurrency-in-action
 */
#ifndef SPIN_LOCK_INC
#define SPIN_LOCK_INC

#include <atomic> 
#include <thread>

/**
 * @brief 自旋锁
 *
 * @note 基于std::atomic_flag实现
 */
class SpinLock {
private:
    std::atomic_flag flag;

public:
    /**
     * @brief 创建一个自旋锁, 初始化为清除状态
     */
    SpinLock(): flag{ATOMIC_FLAG_INIT} {}

    /**
     * @brief 获取锁
     */
    void lock() 
    {
        // 获取flag之前状态并同时设置flag为设置状态,
        // 如果之前为清除状态, 说明获取锁成功, 否则继续自旋
        while (flag.test_and_set(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    /**
     * @brief 释放锁
     */
    void unlock()
    {
        // 设置flag为清除状态
        flag.clear(std::memory_order_release);
    }

    /**
     * @brief 尝试获取锁
     *
     * @return 获取锁成功返回true, 否则返回false
     */
    bool try_lock()
    {
        return !flag.test_and_set(std::memory_order_acquire);
    }
};

#endif
