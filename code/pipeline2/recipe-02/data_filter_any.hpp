#pragma once

#include <boost/any.hpp>
#include "process_node.hpp"

class DataFilterAny: public ProcessNode {
public:
    DataFilterAny();
    ~DataFilterAny() override;

    void setInPipeAny(boost::any pipe);
    void setOutPipeAny(boost::any pipe);
    boost::any getInPipeAny();
    boost::any getOutPipeAny();

private:
    boost::any in_pipe;
    boost::any out_pipe;
};
