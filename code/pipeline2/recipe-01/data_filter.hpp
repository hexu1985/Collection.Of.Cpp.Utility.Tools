#pragma once

#include "process_node.hpp"
#include "pipe.hpp"

template <typename IT, typename OT>
class DataFilter: public ProcessNode {
public:
    DataFilter() = default;
    ~DataFilter() = default;

    void setInput(Pipe<IT> pipe) {
        input = pipe;
    }

    void setOutput(Pipe<OT> pipe) {
        output = pipe;
    }

protected:
    void getData(IT& value)
    {
        input->pop(value);
    }

    void putData(OT value)
    {
        output->push(std::move(value));
    }

private:
    Pipe<IT> input;
    Pipe<OT> output;
};
