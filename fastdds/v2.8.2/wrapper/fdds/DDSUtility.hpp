#pragma once

#include <iostream>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

namespace fdds {

std::ostream& operator<< (std::ostream& out, const eprosima::fastdds::dds::DataWriterQos& qos);

}   // namespace fdds
