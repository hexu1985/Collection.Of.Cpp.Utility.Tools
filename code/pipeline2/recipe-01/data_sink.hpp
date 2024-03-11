#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename T>
class DataSink: public ProcessNode {
public:
    DataSink() = default;
    ~DataSink() = default;

    virtual void setInPipe(Pipe<T> pipe) {
        in_pipe = pipe;
    }

    virtual Pipe<T> getInPipe() {
        return in_pipe;
    }

private:
    Pipe<T> in_pipe;
};

