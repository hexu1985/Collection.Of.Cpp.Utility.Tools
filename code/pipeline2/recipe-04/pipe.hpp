#pragma once

#include <memory>
#include "limitedsize_queue.hpp"

template <typename T>
using Pipe = std::shared_ptr<limitedsize_queue<T>>;

template <typename T>
Pipe<T> make_pipe(size_t max_size) {
    return Pipe<T>(new limitedsize_queue<T>(max_size));
}

