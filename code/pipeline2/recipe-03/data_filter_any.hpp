#pragma once

#include <boost/any.hpp>
#include "process_node.hpp"

class DataFilterAny: public ProcessNode {
public:
    DataFilterAny();
    ~DataFilterAny() override;

    virtual void setInPipeAny(boost::any pipe);
    virtual void setOutPipeAny(boost::any pipe);
    virtual boost::any getInPipeAny();
    virtual boost::any getOutPipeAny();

protected:
    boost::any in_pipe;
    boost::any out_pipe;
};
