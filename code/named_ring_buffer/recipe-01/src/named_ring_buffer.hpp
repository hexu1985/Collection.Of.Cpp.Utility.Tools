#pragma once

#include "interprocess_once.hpp"
#include "interprocess_ring_buffer.hpp"

template <typename T, size_t N>
class NamedRingBuffer {
public:
    NamedRingBuffer(const char* name) {
        InterprocessOnceFlag& once_flag = impl_.get().once_flag;
        InterprocessRingBuffer<T, N>* ring_buffer = &impl_.get().ring_buffer;
        interprocess_call_once(once_flag, [ring_buffer]() { new (ring_buffer) InterprocessRingBuffer<T, N>(); });
    }

    void put(const T& value) {
        InterprocessRingBuffer<T, N>& ring_buffer = impl_.get().ring_buffer;
        ring_buffer.put(value);
    }

    void get(T& value) {
        InterprocessRingBuffer<T, N>& ring_buffer = impl_.get().ring_buffer;
        ring_buffer.get(value);
    }

    bool try_get(T& value) {
        InterprocessRingBuffer<T, N>& ring_buffer = impl_.get().ring_buffer;
        return ring_buffer.try_get(value);
    }

private:
    struct Impl {
        InterprocessOnceFlag once_flag;
        InterprocessRingBuffer<T, N> ring_buffer;
    };

    SharedMemory<Impl> impl_;
};
