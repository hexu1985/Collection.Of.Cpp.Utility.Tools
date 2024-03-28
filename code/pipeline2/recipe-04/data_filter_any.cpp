#include "data_filter_any.hpp"

DataFilterAny::DataFilterAny() {
}

DataFilterAny::~DataFilterAny() {
}

void DataFilterAny::setInPipeAny(boost::any pipe) {
    in_pipe = pipe;
}

void DataFilterAny::setOutPipeAny(boost::any pipe) {
    out_pipe = pipe;
}

boost::any DataFilterAny::getInPipeAny() {
    return in_pipe;
}

boost::any DataFilterAny::getOutPipeAny() {
    return out_pipe;
}
