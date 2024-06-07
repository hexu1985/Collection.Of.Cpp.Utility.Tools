#pragma once

#include <stdexcept>

template <typename T>
class MemoryPool;

template <typename T>
class MemoryBlock {
public:
    constexpr static size_t DEFAULT_BLOCK_SIZE = 1024;
    constexpr static size_t N = (DEFAULT_BLOCK_SIZE > sizeof(T)) ? (DEFAULT_BLOCK_SIZE / sizeof(T)) : 1;

    struct alignas(T) MemoryChunk {
        char storage[sizeof(T)];    
    };

    MemoryBlock(MemoryBlock* nextBlock): next(nextBlock) {
        for (size_t i = 0; i < N; i++) {
            available[i] = i;
        }
    }

    ~MemoryBlock() {}

    bool isFull() const {
        return top == N;
    }

    bool isContain(void* someElement) {
        const MemoryChunk* ptr = static_cast<const MemoryChunk*>(someElement);
        if (elements <= ptr && ptr <= elements + N - 1) {
            return true;
        } else {
            return false;
        }
    }

    void* alloc(size_t size) {
        if (isFull()) {
            throw std::runtime_error("Current block is full");
        }
        size_t idx = available[top];
        top += 1;
        return &elements[idx];
    }

    void free(void* someElement) {
        const MemoryChunk* ptr = static_cast<const MemoryChunk*>(someElement);
        size_t idx = (ptr - elements) / sizeof(MemoryChunk);
        if (idx < N) {
            if (top) {
                top -= 1;
                available[top] = idx;
            } else {
                throw std::runtime_error("Some chunk was freed more than once");
            }
        } else {
            throw std::runtime_error("Freeing chunk that does not belong to the block");
        }
    }

private:
    friend class MemoryPool<T>;

    MemoryBlock* next = nullptr;
    size_t available[N];
    size_t top = 0;
    MemoryChunk elements[N];
};

template <class T>
class MemoryPool {
public:
	MemoryPool();
	virtual ~MemoryPool();

	inline void* alloc(size_t size);
	inline void free(void* someElement);

private:
    MemoryBlock<T>* listOfMemoryBlocks = nullptr;

    MemoryBlock<T>* getFirstNotFullBlock();
    void expandTheListOfMemoryBlocks();
};

template <class T>
MemoryPool<T>::MemoryPool() {
    expandTheListOfMemoryBlocks();
}

template <class T>
MemoryBlock<T>* MemoryPool<T>::getFirstNotFullBlock() {
    for (MemoryBlock<T>* block = listOfMemoryBlocks; block != nullptr; block = block->next) {
        if (!block->isFull()) {
            return block;
        }
    }
    return nullptr;
}

template <class T>
MemoryPool<T>::~MemoryPool() {
	for (MemoryBlock<T>* block = listOfMemoryBlocks; listOfMemoryBlocks != nullptr; block = listOfMemoryBlocks) {
		listOfMemoryBlocks = listOfMemoryBlocks->next;
		delete block;
	}
}

template <class T>
void *MemoryPool<T>::alloc(size_t size) {
    MemoryBlock<T>* block = getFirstNotFullBlock();
    if (!block) {
        expandTheListOfMemoryBlocks();
        block = listOfMemoryBlocks;
    }

    return block->alloc(size);
}

template <class T>
void MemoryPool<T>::free(void *doomed) {
    for (MemoryBlock<T>* block = listOfMemoryBlocks; block != nullptr; block = block->next) {
        if (block->isContain(doomed)) {
            block->free(doomed);
            return;
        }
    }
    throw std::runtime_error("Freeing chunk that does not belong to the pool");
}

template <class T>
void MemoryPool<T>::expandTheListOfMemoryBlocks() {
    MemoryBlock<T>* block = new MemoryBlock<T>(listOfMemoryBlocks);
    listOfMemoryBlocks = block;
}

