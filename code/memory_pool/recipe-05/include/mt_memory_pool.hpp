/**
 * @file mt_memory_pool.hpp
 * @brief 一个支持多线程的内存池包装器
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-25
 *
 * @see 提高C++性能的编程技术, 第7章
 */
#ifndef MT_MEMORY_POOL_INC
#define MT_MEMORY_POOL_INC

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
     * @brief 构造函数
     *
     * @param st_pool单线程版本的内存池
     */
    MTMemoryPool(POOLTYPE &st_pool);

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
    POOLTYPE &stPool;    // 单线程池
    LOCK theLock;
};

template <class M, class L>
MTMemoryPool<M, L>::MTMemoryPool(M &st_pool): stPool(st_pool)
{
}

template <class M, class L>
inline
void *MTMemoryPool<M, L>::alloc(size_t size)
{
    void *mem;

    theLock.lock();
    mem = stPool.alloc(size);
    theLock.unlock();

    return mem;
}

template <class M, class L>
inline
void MTMemoryPool<M, L>::free(void *doomed)
{
    theLock.lock();
    stPool.free(doomed);
    theLock.unlock();
}

#endif
