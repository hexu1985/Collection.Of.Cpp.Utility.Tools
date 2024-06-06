#pragma once

#include <cstddef>
#include <new>

template <class T>
class MemoryPool {
public:
	MemoryPool(size_t size = EXPANSION_SIZE);
	virtual ~MemoryPool();

	inline void *alloc(size_t size);
	inline void free(void *someElement);

private:
    struct MemoryChunk {
        MemoryChunk *next;
    };

    MemoryChunk *freeList = NULL;
	
	enum { EXPANSION_SIZE = 32 };

	void expandTheFreeList(int howMany = EXPANSION_SIZE);
};

template <class T>
MemoryPool<T>::MemoryPool(size_t size)
{
	expandTheFreeList(size);
}

template <class T>
MemoryPool<T>::~MemoryPool()
{
	for (MemoryChunk *nextPtr = freeList; freeList != NULL; nextPtr = freeList) {
		freeList = freeList->next;
		delete [] nextPtr;
	}
}

template <class T>
inline
void *MemoryPool<T>::alloc(size_t)
{
	if (!freeList) {
		expandTheFreeList();
	}

	MemoryChunk *head = freeList;
	freeList = head->next;

	return head;
}

template <class T>
inline
void MemoryPool<T>::free(void *doomed)
{
	MemoryChunk *head = static_cast<MemoryChunk *>(doomed);

	head->next = freeList;
	freeList = head;
}

template <class T>
void MemoryPool<T>::expandTheFreeList(int howMany)
{
	size_t size = (sizeof(T) > sizeof(MemoryChunk *)) ?
		sizeof(T) : sizeof(MemoryChunk *);

	MemoryChunk *runner = reinterpret_cast<MemoryChunk *>(new char[size]);

	freeList = runner;
	for (int i = 0; i < howMany; i++) {
		runner->next =
			reinterpret_cast<MemoryChunk *>(new char[size]);
		runner = runner->next;
	}
	runner->next = NULL;
}

