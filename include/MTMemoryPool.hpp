#ifndef MINI_UTILS_MT_MEMORY_POOL_INC
#define MINI_UTILS_MT_MEMORY_POOL_INC

namespace MiniUtils {

template <class POOLTYPE, class LOCK>
class MTMemoryPool {
public:
    // 从freeList里分配一个元素
    inline void *alloc(size_t size);

    // 返回一个元素给freeList
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
