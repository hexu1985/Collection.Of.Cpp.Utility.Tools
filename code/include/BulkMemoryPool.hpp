/**
 * @file BulkMemoryPool.hpp
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
class BulkMemoryPool {
public:
    /**
     * @brief 创建一个内存池对象
     *
     * @param bulk_size 内存块长度
     * @param expansion_size 每次空闲列表空时, 扩展空闲列表的块个数
     */
    BulkMemoryPool(size_t bulk_size = BULK_SIZE, size_t expansion_size = EXPANSION_SIZE);

    virtual ~BulkMemoryPool();

    /**
     * @brief 从内存池中分配bulkSize_大小的内存
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
        BULK_SIZE = 16,            // 每个bulk的大小, 默认值
        EXPANSION_SIZE = 32        // 如果空闲列表为空, 按该大小扩展它, 默认值
    };

    // 添加空闲元素至空闲列表
    void expandTheFreeList(int howMany = EXPANSION_SIZE);

private:
    const size_t bulkSize_ = BULK_SIZE;        // 每个内存块的大小, 单位为字节
    size_t expansionSize_ = EXPANSION_SIZE;    // 每次扩展空闲列表的bulk个数
    MemoryChunk *chunkList_ = nullptr;        // 空闲列表的下一元素
};

inline void *BulkMemoryPool::alloc(size_t)
{
    if (!chunkList_) {
        expandTheFreeList(expansionSize_);
    }

    MemoryChunk *head = chunkList_;
    chunkList_ = head->next;

    return head;
}

inline void BulkMemoryPool::free(void *doomed)
{
    MemoryChunk *head = static_cast<MemoryChunk *>(doomed);

    head->next = chunkList_;
    chunkList_ = head;
}

}   // namespace mini_util

#endif
