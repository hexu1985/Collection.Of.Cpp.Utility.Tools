#include "pipeline.hpp"

ProcessNode::ProcessNode()
{
}

ProcessNode::~ProcessNode()
{
}

Pipeline::Pipeline()
{
}

Pipeline::~Pipeline()
{
    stop();
    clear();
}

void Pipeline::start()
{
    for (auto sink : data_sinks) {
        sink->start();
    }
    for (auto filter : data_filters) {
        filter->start();
    }
    for (auto source : data_sources) {
        source->start();
    }
}

void Pipeline::stop()
{
    for (auto source : data_sources) {
        source->stop();
    }
    for (auto filter : data_filters) {
        filter->stop();
    }
    for (auto sink : data_sinks) {
        sink->stop();
    }
}

void Pipeline::clear()
{
    data_sources.clear();
    data_filters.clear();
    data_sinks.clear();
}

void Pipeline::add_data_source(std::shared_ptr<ProcessNode> source)
{
    data_sources.push_back(source);
}

void Pipeline::add_data_filter(std::shared_ptr<ProcessNode> filter)
{
    data_filters.push_back(filter);
}

void Pipeline::add_data_sink(std::shared_ptr<ProcessNode> sink)
{
    data_sinks.push_back(sink);
}

