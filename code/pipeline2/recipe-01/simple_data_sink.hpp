#pragma once

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
        while(!done) {
            this->getData(data);
            consume_func(data);
        }
    }

private:
    std::atomic_bool done{false};
    std::function<void(T&)> consume_func;
    std::thread worker_thread;
};

