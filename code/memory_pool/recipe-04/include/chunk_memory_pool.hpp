/**
 * @file chunk_memory_pool.hpp
 * @brief 固定大小的内存池
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-25
 *
 * @see Efficient C++: Performance Programming Techniques, chapter 6.3
 */
#ifndef MINI_UTILS_BULK_MEMORY_POOL_INC
#define MINI_UTILS_BULK_MEMORY_POOL_INC

#include <cstddef>
#include <new>

namespace mini_util {

/**
 * @brief 固定大小的内存池, 每次alloc申请内存的大小固定
 */
class ChunkMemoryPool {
public:
    /**
     * @brief 创建一个内存池对象
     *
     * @param chunk_size 内存块长度
     * @param expansion_size 每次空闲列表空时, 扩展空闲列表的块个数
     */
    ChunkMemoryPool(size_t chunk_size, size_t expansion_size = EXPANSION_SIZE);

    virtual ~ChunkMemoryPool();

    /**
     * @brief 从内存池中分配chunkSize大小的内存
     *
     * @param size 未使用
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
    // 内存块结构
    struct MemoryChunk {
        MemoryChunk *next;
    };

    enum {
        EXPANSION_SIZE = 32        // 如果空闲列表为空, 按该大小扩展它, 默认值
    };

    // 添加空闲元素至空闲列表
    void expandTheFreeList(int howMany = EXPANSION_SIZE);

private:
    const size_t chunkSize;        // 每个内存块的大小, 单位为字节
    size_t expansionSize;          // 每次扩展空闲列表的bulk个数

    // 内存块列表, 它是我们的私有存储空间
    MemoryChunk *listOfMemoryChunks = NULL;
};

inline void *ChunkMemoryPool::alloc(size_t)
{
    if (!listOfMemoryChunks) {
        expandTheFreeList(expansionSize);
    }

    MemoryChunk *head = listOfMemoryChunks;
    listOfMemoryChunks = head->next;

    return head;
}

inline void ChunkMemoryPool::free(void *doomed)
{
    MemoryChunk *head = static_cast<MemoryChunk *>(doomed);

    head->next = listOfMemoryChunks;
    listOfMemoryChunks = head;
}

}   // namespace mini_util

#endif
