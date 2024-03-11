#pragma once

#include <thread>
#include <functional>
#include <atomic>
#include <memory>
#include "data_source.hpp"

template <typename T>
class SimpleDataSource: public DataSource<T> {
public:
    SimpleDataSource(std::function<bool(T&)> func): product_func(func) {}

    ~SimpleDataSource() override {
        stop();
    }

    void start() override {
        if (worker_thread.joinable()) {
            return;
        }
        worker_thread = std::thread(&SimpleDataSource::worker_routine, this);
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
        auto out_pipe = this->getOutPipe();
        while(!done) {
            if (!product_func(data)) {
                break;
            }
            out_pipe->push(std::move(data));
        }
    }

private:
    std::atomic_bool done{false};
    std::function<bool(T&)> product_func;
    std::thread worker_thread;
};

