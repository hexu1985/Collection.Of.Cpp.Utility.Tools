#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <boost/any.hpp>

#include "pipe.hpp"
#include "process_node.hpp"
#include "data_source.hpp"
#include "data_sink.hpp"
#include "data_filter.hpp"

template <typename SourceDataType, typename SinkDataType>
class Pipeline: public ProcessNode {
public:
    Pipeline() {
        source_pipe = make_pipe<SourceDataType>();
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
        data_source->setOutput(source_pipe);
        process_nodes.push_back(data_source);
        return *this;
    }

    template <typename IT, typename OT>
    Pipeline& addDataFilter(std::shared_ptr<DataFilter<IT, OT>> data_filter) {
        assert(sink_pipe == nullptr);
        auto input = boost::any_cast<Pipe<IT>>(pipes.back());
        data_filter->setInput(input);
        auto output = make_pipe<OT>();
        data_filter->setOutput(output);
        pipes.push_back(output);
        process_nodes.push_back(data_filter);
        return *this;
    }

    Pipeline& addDataSink(std::shared_ptr<DataSink<SinkDataType>> data_sink) {
        if (sink_pipe == nullptr) {
            sink_pipe = boost::any_cast<Pipe<SinkDataType>>(pipes.back());
        }
        data_sink->setInput(sink_pipe);
        process_nodes.push_back(data_sink);
        return *this;
    }

    void clear() {
        process_nodes.clear();
        sink_pipe = nullptr;
        pipes.clear();
        pipes.push_back(source_pipe);
    }
    
    void getData(SinkDataType& data) {
        if (sink_pipe == nullptr) {
            sink_pipe = boost::any_cast<Pipe<SinkDataType>>(pipes.back());
        }
        sink_pipe->pop(data);
    }

    void putData(const SourceDataType& data) {
        source_pipe->push(data);
    }

protected:
    std::vector<std::shared_ptr<ProcessNode>> process_nodes;
    Pipe<SourceDataType> source_pipe;
    Pipe<SinkDataType> sink_pipe;
    std::vector<boost::any> pipes;
};

