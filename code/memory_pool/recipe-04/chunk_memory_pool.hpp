#pragma once

#include <cstddef>
#include <new>

class ChunkMemoryPool {
public:
    ChunkMemoryPool(size_t chunk_size, size_t expansion_size = EXPANSION_SIZE);
    virtual ~ChunkMemoryPool();

    inline void *alloc(size_t size);
    inline void free(void *someElement);

private:
    struct MemoryChunk {
        MemoryChunk *next;
    };

    enum {
        EXPANSION_SIZE = 32
    };

    void expandTheFreeList(int howMany = EXPANSION_SIZE);

private:
    const size_t chunkSize;
    size_t expansionSize;

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

