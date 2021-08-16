#include "BulkMemoryPool.hpp"

namespace mini_util {

BulkMemoryPool::BulkMemoryPool(size_t bulk_size, size_t expansion_size): 
	bulkSize_(bulk_size),
    expansionSize_(expansion_size),
    chunkList_(nullptr)
{
	expandTheFreeList(expansionSize_);
}

BulkMemoryPool::~BulkMemoryPool()
{
	for (MemoryChunk *nextPtr = chunkList_; chunkList_ != nullptr; nextPtr = chunkList_) {
		chunkList_ = chunkList_->next;
		delete [] nextPtr;
	}
}

void BulkMemoryPool::expandTheFreeList(int howMany)
{
	// 我们必须分配足够大的对象以包含下一个指针
	size_t size = (bulkSize_ > sizeof(MemoryChunk *)) ?
		bulkSize_ : sizeof(MemoryChunk *);

	MemoryChunk *runner = reinterpret_cast<MemoryChunk *>(new char[size]);

	chunkList_ = runner;
	for (int i = 0; i < howMany; i++) {
		runner->next =
			reinterpret_cast<MemoryChunk *>(new char[size]);
		runner = runner->next;
	}
	runner->next = nullptr;
}

}   // namespace mini_util
