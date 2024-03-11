#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename T>
class DataSource: public ProcessNode {
public:
    DataSource() = default;
    ~DataSource() = default;

    virtual void setOutPipe(Pipe<T> pipe) {
        out_pipe = pipe;
    }

    virtual Pipe<T> getOutPipe() {
        return out_pipe;
    }

private:
    Pipe<T> out_pipe;
};

