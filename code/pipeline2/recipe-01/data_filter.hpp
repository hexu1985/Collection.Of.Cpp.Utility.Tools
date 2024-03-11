#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename IT, typename OT>
class DataFilter: public ProcessNode {
public:
    DataFilter() = default;
    ~DataFilter() = default;

    void setInPipe(Pipe<IT> pipe) {
        in_pipe = pipe;
    }

    void setOutPipe(Pipe<OT> pipe) {
        out_pipe = pipe;
    }

    Pipe<IT> getInPipe() {
        return in_pipe;
    }

    Pipe<OT> getOutPipe() {
        return out_pipe;
    }

private:
    Pipe<IT> in_pipe;
    Pipe<OT> out_pipe;
};
