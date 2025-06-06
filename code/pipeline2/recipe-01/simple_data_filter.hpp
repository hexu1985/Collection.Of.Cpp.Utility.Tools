#pragma once

#include <thread>
#include <functional>
#include <atomic>
#include "data_filter.hpp"

template <typename IT, typename OT>
class SimpleDataFilter: public DataFilter<IT,OT> {
public:
    SimpleDataFilter(std::function<OT(IT)> func): filter_func(func) {}

    ~SimpleDataFilter() override {
        stop();
    }

    void start() override {
        if (worker_thread.joinable()) {
            return;
        }
        done = false;
        worker_thread = std::thread(&SimpleDataFilter::worker_routine, this);
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
        IT input_data;
        OT output_data;
        auto in_pipe = this->getInPipe();
        auto out_pipe = this->getOutPipe();
        while(!done) {
            in_pipe->pop(input_data);
            output_data = filter_func(std::move(input_data));
            out_pipe->push(std::move(output_data));
        }
    }

private:
    std::atomic_bool done{false};
    std::function<OT(IT)> filter_func;
    std::thread worker_thread;
};
