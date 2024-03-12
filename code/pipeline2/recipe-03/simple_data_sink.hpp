#pragma once

#include <memory>
#include <thread>
#include <functional>
#include <atomic>
#include "data_sink.hpp"

template <typename T>
class SimpleDataSink: public DataSink<T> {
public:
    SimpleDataSink(std::function<void(T&)> func): consume_func(func) {}

    ~SimpleDataSink() override {
        stop();
    }

    void start() override {
        if (worker_thread.joinable()) {
            return;
        }
        worker_thread = std::thread(&SimpleDataSink::worker_routine, this);
    }

    void stop() override {
        if (!worker_thread.joinable()) {
            return;
        }
        done = true;
        worker_thread.join();
    }

private:
    void worker_routine() {
        T data;
        auto in_pipe = this->getInPipe();
        while(!done) {
            in_pipe->pop(data);
            consume_func(data);
        }
    }

private:
    std::atomic_bool done{false};
    std::function<void(T&)> consume_func;
    std::thread worker_thread;
};

template <typename T>
std::shared_ptr<DataSink<T>> make_simple_data_sink(std::function<void(T&)> func) {
    return std::make_shared<SimpleDataSink<T>>(func);
}
