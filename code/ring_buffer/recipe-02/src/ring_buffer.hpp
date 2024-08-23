#pragma once

#include <stdlib.h>

template<class T>
class RingBuffer {
private:
    struct BufferHead {
        size_t read;
        size_t write;
        size_t queued;
        size_t capacity;
        T objects[];
    };

    BufferHead* buffer_ = nullptr; 

public:
    RingBuffer(size_t N) {
        buffer_ = (BufferHead*) malloc(sizeof(BufferHead) + sizeof(T)*N);
        buffer_->read = 0;
        buffer_->write = 0;
        buffer_->queued = 0;
        buffer_->capacity = N;
    }

    ~RingBuffer() {
        free(buffer_);
    }

    T& push() {
        T& current = buffer_->objects[buffer_->write];
        buffer_->write = (buffer_->write + 1) % buffer_->capacity;
        buffer_->queued++;
        if (buffer_->queued > buffer_->capacity) {
            buffer_->queued = buffer_->capacity;
            buffer_->read = buffer_->write;
        }
        return current;
    }

    const T& pull() {
        if (is_empty()) {
            throw std::runtime_error("No data in the ring buffer");
        }
        T& current = buffer_->objects[buffer_->read];
        buffer_->read = (buffer_->read + 1) % buffer_->capacity;
        buffer_->queued--;
        return current;
    }

    bool is_empty() {
        return buffer_->queued == 0;
    }

    bool is_full() {
        return buffer_->queued == buffer_->capacity;
    }

    size_t size() {
        return buffer_->queued;
    }

    bool has_data() {
        return buffer_->queued != 0;
    }
};
