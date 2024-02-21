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
    for (auto process_node : process_nodes) {
        process_node->start();
    }
}

void Pipeline::stop()
{
    for (auto process_node : process_nodes) {
        process_node->stop();
    }
}

void Pipeline::clear()
{
    process_nodes.clear();
}

void Pipeline::add_process_node(std::shared_ptr<ProcessNode> process_node)
{
    process_nodes.push_back(process_node);
}


