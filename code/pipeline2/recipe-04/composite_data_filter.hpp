#pragma once

#include <vector>
#include <memory>
#include <boost/any.hpp>

#include "data_filter_any.hpp"
#include "data_filter.hpp"
#include "pipe.hpp"

template <typename IT, typename OT>
class CompositeDataFilter: public DataFilterAny {
public:
    using Base = DataFilterAny;

    CompositeDataFilter(size_t capacity_per_pipe_): capacity_per_pipe(capacity_per_pipe_) {
    }

    ~CompositeDataFilter() override {
        stop();
    }

    void start() override {
        for (auto data_filter : data_filters) {
            data_filter->start();
        }
    }

    void stop() override {
        for (auto data_filter : data_filters) {
            data_filter->stop();
        }
    }

    CompositeDataFilter& addDataFilterAny(std::shared_ptr<DataFilterAny> data_filter, boost::any next_pipe) {
        assert(!isSetOutPipe());
        if (data_filters.empty()) {     // the first sub filter
            data_filter->setInPipeAny(getInPipeAny());
        } else {
            data_filter->setInPipeAny(data_filters.back()->getOutPipeAny());
        }
        data_filter->setOutPipeAny(next_pipe);
        data_filters.push_back(data_filter);
        return *this;
    }

    template <typename IT2, typename OT2>
    CompositeDataFilter& addDataFilter(std::shared_ptr<DataFilter<IT2, OT2>> data_filter) {
        addDataFilterAny(data_filter, make_pipe<OT2>(capacity_per_pipe));
        return *this;
    }

    template <typename IT2, typename OT2>
    CompositeDataFilter& addDataFilter(std::shared_ptr<CompositeDataFilter<IT2, OT2>> data_filter) {
        addDataFilterAny(data_filter, make_pipe<OT2>(capacity_per_pipe));
        return *this;
    }

    void setInPipeAny(boost::any pipe) override {
        Base::setInPipeAny(pipe);
        if (data_filters.empty()) {
            return;
        }
        data_filters.front()->setInPipeAny(pipe);
    }

    void setOutPipeAny(boost::any pipe) override {
        Base::setOutPipeAny(pipe);
        if (data_filters.empty()) {
            return;
        }
        data_filters.back()->setOutPipeAny(pipe);
    }

    void setInPipe(Pipe<IT> pipe) {
        setInPipeAny(pipe);
    }

    void setOutPipe(Pipe<OT> pipe) {
        setOutPipeAny(pipe);
    }

    Pipe<IT> getInPipe() {
        auto pipe_any = this->getInPipeAny();
        return boost::any_cast<Pipe<IT>>(pipe_any);
    }

    Pipe<OT> getOutPipe() {
        auto pipe_any = this->getOutPipeAny();
        return boost::any_cast<Pipe<OT>>(pipe_any);
    }

    bool isSetInPipe() const {
        return !in_pipe.empty();
    }

    bool isSetOutPipe() const {
        return !out_pipe.empty();
    }

protected:
    std::vector<std::shared_ptr<DataFilterAny>> data_filters;
    size_t capacity_per_pipe;
};

template <typename IT, typename OT>
std::shared_ptr<CompositeDataFilter<IT, OT>> make_composite_data_filter(size_t capacity_per_pipe) {
    return std::make_shared<CompositeDataFilter<IT, OT>>(capacity_per_pipe);
}
