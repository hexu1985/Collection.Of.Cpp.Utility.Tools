#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename T>
class DataSink: public ProcessNode {
public:
    DataSink() = default;
    ~DataSink() = default;

    void setInput(Pipe<T> pipe) {
        input = pipe;
    }

protected:
    void getData(T& value)
    {
        input->pop(value);
    }

private:
    Pipe<T> input;
};

