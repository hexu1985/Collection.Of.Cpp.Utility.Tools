#pragma once

#include <stdexcept>

template <typename T, size_t N>
class MemoryPool {
public:
    struct alignas(T) MemoryChunk {
        char storage[sizeof(T)];    
    };

    MemoryPool() {
        for (size_t i = 0; i < N; i++) {
            available[i] = i;
        }
    }

    ~MemoryPool() {}

    bool isFull() const {
        return top == N;
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
    MemoryChunk elements[N];
    size_t available[N];
    size_t top = 0;
};

