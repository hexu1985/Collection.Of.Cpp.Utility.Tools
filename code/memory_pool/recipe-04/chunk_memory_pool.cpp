#include "chunk_memory_pool.hpp"

ChunkMemoryPool::ChunkMemoryPool(size_t chunk_size, size_t expansion_size): 
	chunkSize(chunk_size),
    expansionSize(expansion_size),
    listOfMemoryChunks(NULL)
{
	expandTheFreeList(expansionSize);
}

ChunkMemoryPool::~ChunkMemoryPool()
{
	for (MemoryChunk *nextPtr = listOfMemoryChunks; listOfMemoryChunks != NULL; nextPtr = listOfMemoryChunks) {
		listOfMemoryChunks = listOfMemoryChunks->next;
		delete [] nextPtr;
	}
}

void ChunkMemoryPool::expandTheFreeList(int howMany)
{
	size_t size = (chunkSize > sizeof(MemoryChunk *)) ?
		chunkSize : sizeof(MemoryChunk *);

	MemoryChunk *runner = reinterpret_cast<MemoryChunk *>(new char[size]);

	listOfMemoryChunks = runner;
	for (int i = 0; i < howMany; i++) {
		runner->next =
			reinterpret_cast<MemoryChunk *>(new char[size]);
		runner = runner->next;
	}
	runner->next = NULL;
}

