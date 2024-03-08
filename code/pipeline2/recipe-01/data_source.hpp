#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename T>
class DataSource: public ProcessNode {
public:
    DataSource() = default;
    ~DataSource() = default;

    void setOutput(Pipe<T> pipe) {
        output = pipe;
    }

protected:
    void putData(T value) {
        assert(output);
        output->push(std::move(value));
    }

private:
    Pipe<T> output;
};

