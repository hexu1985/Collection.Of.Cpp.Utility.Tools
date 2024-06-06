#include "byte_memory_pool.hpp"

ByteMemoryPool::MemoryChunk::MemoryChunk(MemoryChunk* nextChunk, size_t reqSize) {
    chunkSize = (reqSize > DEFAULT_CHUNK_SIZE) ? reqSize : DEFAULT_CHUNK_SIZE;
    next = nextChunk;
    bytesAlreadyAllocated = 0;
    mem = new char [chunkSize];
}

ByteMemoryPool::MemoryChunk::~MemoryChunk() { delete [] mem; }

ByteMemoryPool::ByteMemoryPool(size_t initSize) {
    expandStorage(initSize);
}

ByteMemoryPool::~ByteMemoryPool() {
    MemoryChunk* memChunk = listOfMemoryChunks;

    while (memChunk) {
        listOfMemoryChunks = memChunk->nextMemoryChunk();
        delete memChunk;
        memChunk = listOfMemoryChunks;
    }
}

void ByteMemoryPool::expandStorage(size_t reqSize) {
    listOfMemoryChunks = new MemoryChunk(listOfMemoryChunks, reqSize);
}

