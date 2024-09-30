#pragma once

#include "interprocess_once.hpp"
#include "interprocess_ring_buffer.hpp"

template <typename T, size_t N>
class NamedRingBuffer {
public:
    NamedRingBuffer(const char* name) {
    }

private:
    struct Impl {
        InterprocessOnceFlag once_flag;
        InterprocessRingBuffer ring_buffer;
    };

    SharedMemory<Impl> impl_;
};
