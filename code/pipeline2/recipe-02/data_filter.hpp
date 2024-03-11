#pragma once

#include "data_filter_any.hpp"
#include "pipe.hpp"

template <typename IT, typename OT>
class DataFilter: public DataFilterAny {
public:
    DataFilter() = default;
    ~DataFilter() = default;

    void setInPipe(Pipe<IT> pipe) {
        this->setInPipeAny(pipe);
    }

    void setOutPipe(Pipe<OT> pipe) {
        this->setOutPipeAny(pipe);
    }

    Pipe<IT> getInPipe() {
        auto pipe_any = this->getInPipeAny();
        return boost::any_cast<Pipe<IT>>(pipe_any);
    }

    Pipe<OT> getOutPipe() {
        auto pipe_any = this->getOutPipeAny();
        return boost::any_cast<Pipe<OT>>(pipe_any);
    }
};
