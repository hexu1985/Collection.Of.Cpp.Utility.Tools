#ifndef MINI_UTILS_BULK_MEMORY_POOL_INC
#define MINI_UTILS_BULK_MEMORY_POOL_INC

#include <cstddef>
#include <new>

namespace MiniUtils {

class BulkMemoryPool {
public:
	BulkMemoryPool(size_t bulk_size = BULK_SIZE, size_t expansion_size = EXPANSION_SIZE);
	~BulkMemoryPool();

	// 从空闲列表中分配bulkSize_字节大小的元素
	inline void *alloc(size_t size);

	// 返回bulkSize_字节大小的元素到空闲列表中
	inline void free(void *someElement);

private:
	// 内存块
    struct MemoryChunk {
        MemoryChunk *next;
    };

	enum {
		BULK_SIZE = 16,			// 每个bulk的大小, 默认值
		EXPANSION_SIZE = 32		// 如果空闲列表为空, 按该大小扩展它, 默认值
	};

	// 添加空闲元素至空闲列表
	void expandTheFreeList(int howMany = EXPANSION_SIZE);

private:
	const size_t bulkSize_ = BULK_SIZE;		// 每个bulk的大小, 单位为字节
    size_t expansionSize_ = EXPANSION_SIZE;	// 每次扩展空闲列表的bulk个数
    MemoryChunk *chunkList_ = nullptr;		// 空闲列表的下一元素
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

}   // namespace MiniUtils

#endif
