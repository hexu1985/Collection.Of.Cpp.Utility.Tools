/**
 * @file MTMemoryPool.hpp
 * @brief 一个支持多线程的内存池
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-25
 *
 * @see Efficient C++: Performance Programming Techniques, chapter 7
 */
#ifndef MINI_UTILS_MT_MEMORY_POOL_INC
#define MINI_UTILS_MT_MEMORY_POOL_INC

namespace MiniUtils {

/**
 * @brief 线程安全的内存池
 *
 * @tparam POOLTYPE 单线程版内存池类型
 * @tparam LOCK 锁类型
 */
template <class POOLTYPE, class LOCK>
class MTMemoryPool {
public:
    /**
     * @brief 从内存池中分配size大小的内存
     *
     * @param size 申请内存的大小
     *
     * @return 返回分配内存的指针
     */
    inline void *alloc(size_t size);

    /**
     * @brief 释放先前从内存池中分配的内存
     *
     * @param someElement 被释放的指针
     */
    inline void free(void *someElement);

private:
    POOLTYPE stPool_;    // 单线程池
    LOCK theLock_;
};

template <class M, class L>
inline
void *MTMemoryPool<M, L>::alloc(size_t size)
{
    void *mem;

    theLock_.lock();
    mem = stPool_.alloc(size);
    theLock_.unlock();

    return mem;
}

template <class M, class L>
inline
void MTMemoryPool<M, L>::free(void *doomed)
{
    theLock_.lock();
    stPool_.free(doomed);
    theLock_.unlock();
}

}   // namespace MiniUtils

#endif
