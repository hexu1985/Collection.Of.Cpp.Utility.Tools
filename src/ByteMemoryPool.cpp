#include "ByteMemoryPool.hpp"

namespace MiniUtils {

ByteMemoryPool::MemoryChunk::MemoryChunk(MemoryChunk *nextChunk, size_t reqSize)
{
    chunkSize_ = (reqSize > DEFAULT_CHUNK_SIZE) ? reqSize : DEFAULT_CHUNK_SIZE;
    next_ = nextChunk;
    bytesAlreadyAllocated_ = 0;
    data_ = new char [chunkSize_];
}

ByteMemoryPool::MemoryChunk::~MemoryChunk() { delete [] data_; }

// 创建ByteMemoryPool对象, 生成私有存储空间
ByteMemoryPool::ByteMemoryPool(size_t initSize)
{
    expandStorage(initSize);
}

ByteMemoryPool::~ByteMemoryPool()
{
    MemoryChunk *memChunk = listOfMemoryChunks_;

    while (memChunk) {
        listOfMemoryChunks_ = memChunk->nextMemoryChunk();
        delete memChunk;
        memChunk = listOfMemoryChunks_;
    }
}

void ByteMemoryPool::expandStorage(size_t reqSize)
{
    listOfMemoryChunks_ = new MemoryChunk(listOfMemoryChunks_, reqSize);
}

}   // namespace MiniUtils
