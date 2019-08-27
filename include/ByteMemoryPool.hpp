/**
 * @file ByteMemoryPool.hpp
 * @brief 可变大小的内存池
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-25
 *
 * @see Efficient C++: Performance Programming Techniques, chapter 6.4
 */
#ifndef MINI_UTILS_BYTE_MEMORY_POOL_INC
#define MINI_UTILS_BYTE_MEMORY_POOL_INC

#include <cstddef>
#include <new>

namespace mini_utils {

/**
 * @brief 可变大小的内存池, 每次alloc申请内存的大小可变
 */
class ByteMemoryPool {
public:
    /**
     * @brief 创建一个内存池
     *
     * @param initSize 内存块大小
     */
    ByteMemoryPool(size_t initSize = DEFAULT_CHUNK_SIZE);

    ~ByteMemoryPool();

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
    // 这是一个内存块的默认大小
    enum { DEFAULT_CHUNK_SIZE = 4096 };

    class MemoryChunk {
    public:
        MemoryChunk(MemoryChunk *nextChunk, size_t chunkSize);
        ~MemoryChunk();

        void *alloc(size_t size)
        {
            void *addr = static_cast<void *>
                (static_cast<char *>(data_) + bytesAlreadyAllocated_);
            bytesAlreadyAllocated_ += size;

            return addr;
        }

        void free(void *someElement) {}

        // 指向列表下一内存块的指针
        MemoryChunk *nextMemoryChunk() { return next_; }

        // 当前内存块剩余空间大小
        size_t spaceAvailable()
        {
            return chunkSize_ - bytesAlreadyAllocated_;
        }

    private:
        MemoryChunk *next_;             // 自由链表的下一个内存块
        char *data_;                    // 实际存储内存块的空间
        size_t chunkSize_;              // 一个内存块的大小
        size_t bytesAlreadyAllocated_;  // 当前内存块中已分配的字节数
    };

private:
    // 内存块列表, 它是我们的私有存储空间
    MemoryChunk *listOfMemoryChunks_ = nullptr;

    // 向我们的私有存储空间添加一个内存块
    void expandStorage(size_t reqSize);
};

inline void *ByteMemoryPool::alloc(size_t requestSize)
{
    size_t space = listOfMemoryChunks_->spaceAvailable();
    if (space < requestSize) {
        expandStorage(requestSize);
    }

    return listOfMemoryChunks_->alloc(requestSize);
}

inline void ByteMemoryPool::free(void *doomed)
{
    listOfMemoryChunks_->free(doomed);
}

}   // namespace mini_utils

#endif
