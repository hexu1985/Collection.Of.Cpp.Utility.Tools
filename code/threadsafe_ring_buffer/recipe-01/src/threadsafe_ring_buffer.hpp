#pragma once

#include "ring_buffer.hpp"

#include <mutex>
#include <condition_variable>

template <typename T, size_t N>
class ThreadsafeRingBuffer {
private:
    std::mutex mtx_;
    std::condition_variable cv_;
    RingBuffer<T, N> buffer_;

public:
    ThreadsafeRingBuffer() {}

    void put(const T& value) {
        std::lock_guard<std::mutex> lck(mtx_);
        buffer_.put(value);
        cv_.notify_one();
    }

    void get(T& value) {
        std::unique_lock<std::mutex> lck(mtx_);
        cv_.wait(lck, [this] { return buffer_.has_data(); });
        buffer_.get(value);
    }

    bool try_get(T& value) {
        std::lock_guard<std::mutex> lck(mtx_);
        if (buffer_.has_data()) {
            buffer_.get(value);
            return true;
        } else {
            return false;
        }
    }
};
