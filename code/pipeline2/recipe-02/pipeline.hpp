#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <boost/any.hpp>

#include "pipe.hpp"
#include "process_node.hpp"
#include "data_source.hpp"
#include "data_sink.hpp"
#include "data_filter_any.hpp"

template <typename SourceDataType, typename SinkDataType>
class Pipeline: public ProcessNode {
public:
    Pipeline() {
        source_pipe = make_pipe<SourceDataType>();
        pipes.push_back(source_pipe);
    }

    Pipeline(Pipe<SourceDataType> source_pipe_): source_pipe(source_pipe_) {
        pipes.push_back(source_pipe);
    }

    ~Pipeline() override {
        stop();
        clear();
    }

    void start() override {
        for (auto process_node : process_nodes) {
            process_node->start();
        }
    }

    void stop() override {
        for (auto process_node : process_nodes) {
            process_node->stop();
        }
    }

    Pipeline& addDataSource(std::shared_ptr<DataSource<SourceDataType>> data_source) {
        data_source->setOutPipe(source_pipe);
        process_nodes.push_back(data_source);
        return *this;
    }

    template <typename OT>
    Pipeline& addDataFilterAny(std::shared_ptr<DataFilterAny> data_filter, OT* pot) {
        (void)pot;
        assert(sink_pipe == nullptr);
        auto in_pipe = pipes.back();
        data_filter->setInPipeAny(in_pipe);
        auto out_pipe = make_pipe<OT>();
        data_filter->setOutPipeAny(out_pipe);
        pipes.push_back(out_pipe);
        process_nodes.push_back(data_filter);
        return *this;
    }

    Pipeline& addDataSink(std::shared_ptr<DataSink<SinkDataType>> data_sink) {
        if (sink_pipe == nullptr) {
            sink_pipe = boost::any_cast<Pipe<SinkDataType>>(pipes.back());
        }
        data_sink->setInPipe(sink_pipe);
        process_nodes.push_back(data_sink);
        return *this;
    }

    void clear() {
        process_nodes.clear();
        sink_pipe = nullptr;
        pipes.clear();
        pipes.push_back(source_pipe);
    }

    Pipe<SourceDataType> getSourcePipe() {
        return source_pipe;
    }

    Pipe<SinkDataType> getSinkPipe() {
        if (sink_pipe == nullptr) {
            sink_pipe = boost::any_cast<Pipe<SinkDataType>>(pipes.back());
        }
        return sink_pipe;
    }
    
protected:
    std::vector<std::shared_ptr<ProcessNode>> process_nodes;
    Pipe<SourceDataType> source_pipe;
    Pipe<SinkDataType> sink_pipe;
    std::vector<boost::any> pipes;
};

