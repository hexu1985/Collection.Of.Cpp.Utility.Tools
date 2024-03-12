#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename T>
class DataSink: public ProcessNode {
public:
    DataSink() = default;
    ~DataSink() = default;

    void setInPipe(Pipe<T> pipe) {
        in_pipe = pipe;
    }

    Pipe<T> getInPipe() {
        return in_pipe;
    }

private:
    Pipe<T> in_pipe;
};

