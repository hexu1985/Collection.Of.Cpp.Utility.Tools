#pragma once

#include "ring_buffer.hpp"
#include "interprocess_mutex.hpp"
#include "interprocess_condition.hpp"
#include <mutex>

template <typename T, size_t N>
class InterprocessRingBuffer {
private:
    InterprocessMutex mtx_;
    InterprocessCondition cv_;
    RingBuffer<T, N> buffer_;

public:
    InterprocessRingBuffer() {}

    void put(const T& value) {
        std::lock_guard<InterprocessMutex> lck(mtx_);
        buffer_.put(value);
        cv_.notify_one();
    }

    void get(T& value) {
        std::unique_lock<InterprocessMutex> lck(mtx_);
        cv_.wait(lck, [this] { return buffer_.has_data(); });
        buffer_.get(value);
    }

    bool try_get(T& value) {
        std::lock_guard<InterprocessMutex> lck(mtx_);
        if (buffer_.has_data()) {
            buffer_.get(value);
            return true;
        } else {
            return false;
        }
    }
};
