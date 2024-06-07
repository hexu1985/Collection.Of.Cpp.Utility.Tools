#pragma once

template <class POOLTYPE, class LOCK>
class MTMemoryPool {
public:
    MTMemoryPool(POOLTYPE &st_pool);

    inline void* alloc(size_t size);
    inline void free(void* someElement);

private:
    POOLTYPE &stPool;    // 单线程池
    LOCK theLock;
};

template <class M, class L>
MTMemoryPool<M, L>::MTMemoryPool(M &st_pool): stPool(st_pool) {
}

template <class M, class L>
inline
void* MTMemoryPool<M, L>::alloc(size_t size) {
    void* mem;

    theLock.lock();
    mem = stPool.alloc(size);
    theLock.unlock();

    return mem;
}

template <class M, class L>
inline
void MTMemoryPool<M, L>::free(void* doomed) {
    theLock.lock();
    stPool.free(doomed);
    theLock.unlock();
}

