#pragma once

#include <cstdint>
#include <stdexcept>
#include "shared_memory.hpp"
#include "interprocess_once.hpp"
#include "interprocess_ring_buffer.hpp"

#define FMT_HEADER_ONLY
#include "fmt/format.h"

#define NAMED_RING_BUFFER_MAGIC 0x78231459

template <typename T, size_t N>
class NamedRingBuffer {
public:
    NamedRingBuffer(const char* name): impl_(name) {
        InterprocessOnceFlag& once_flag = impl_.get().once_flag;
        InterprocessRingBuffer<T, N>* ring_buffer = &impl_.get().ring_buffer;
        uint32_t* magic = &impl_.get().magic;
        size_t* type_size = &impl_.get().type_size;
        size_t* buffer_size = &impl_.get().buffer_size;
        interprocess_call_once(once_flag, [ring_buffer, magic, type_size, buffer_size]() { 
                new (ring_buffer) InterprocessRingBuffer<T, N>(); 
                *magic = NAMED_RING_BUFFER_MAGIC;
                *type_size = sizeof(T);
                *buffer_size = N;
                });
        check_ring_buffer_valid(*magic, *type_size, *buffer_size);
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
    void check_ring_buffer_valid(uint32_t magic, size_t type_size, size_t buffer_size) {
        if (magic != NAMED_RING_BUFFER_MAGIC) {
            throw std::runtime_error(
                    fmt::format("check_ring_buffer_valid failed, invalid magic: expect [{}], in fact [{}] ", (uint32_t) NAMED_RING_BUFFER_MAGIC, magic)
                    );
        }

        if (type_size != sizeof(T)) {
            throw std::runtime_error(
                    fmt::format("check_ring_buffer_valid failed, invalid type size: expect [{}], in fact [{}] ", sizeof(T), type_size)
                    );
        }

        if (buffer_size != N) {
            throw std::runtime_error(
                    fmt::format("check_ring_buffer_valid failed, invalid buffer size: expect [{}], in fact [{}] ", N, buffer_size)
                    );
        }
    }

private:
    struct Impl {
        InterprocessOnceFlag once_flag;
        uint32_t magic;
        size_t type_size;
        size_t buffer_size; 
        InterprocessRingBuffer<T, N> ring_buffer;
    };

    SharedMemory<Impl> impl_;
};
