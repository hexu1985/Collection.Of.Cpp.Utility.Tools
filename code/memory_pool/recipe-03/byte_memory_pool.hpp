#pragma once

#include <cstddef>
#include <new>

class ByteMemoryPool {
public:
    ByteMemoryPool(size_t initSize = DEFAULT_CHUNK_SIZE);

    ~ByteMemoryPool();

    inline void *alloc(size_t size);
    inline void free(void *someElement);

private:
    enum { DEFAULT_CHUNK_SIZE = 4096 };

    class MemoryChunk {
    public:
        MemoryChunk(MemoryChunk *nextChunk, size_t chunkSize);
        ~MemoryChunk();

        void *alloc(size_t size)
        {
            void *addr = static_cast<void *>(static_cast<char *>(mem) + bytesAlreadyAllocated);
            bytesAlreadyAllocated += size;

            return addr;
        }

        void free(void *someElement) {}

        MemoryChunk *nextMemoryChunk() { return next; }

        size_t spaceAvailable()
        {
            return chunkSize - bytesAlreadyAllocated;
        }

    private:
        MemoryChunk *next;
        char *mem;
        size_t chunkSize;
        size_t bytesAlreadyAllocated;
    };

private:
    MemoryChunk *listOfMemoryChunks = NULL;

    void expandStorage(size_t reqSize);
};

inline void *ByteMemoryPool::alloc(size_t requestSize)
{
    size_t space = listOfMemoryChunks->spaceAvailable();
    if (space < requestSize) {
        expandStorage(requestSize);
    }

    return listOfMemoryChunks->alloc(requestSize);
}

inline void ByteMemoryPool::free(void *doomed)
{
    listOfMemoryChunks->free(doomed);
}

