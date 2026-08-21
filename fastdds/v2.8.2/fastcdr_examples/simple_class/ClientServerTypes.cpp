#include "ClientServerTypes.hpp"
#include <fastcdr/FastBuffer.h>
#include <cstring>

using namespace clientserver;

bool Operation::SerializeToVector(std::vector<uint8_t>& output) const {
    output.resize(1024);
    eprosima::fastcdr::FastBuffer fast_buffer((char*) output.data(), output.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    cdr << (uint32_t) m_operationType << m_num1 << m_num2; 
    output.resize(cdr.getSerializedDataLength());

    return true;
}

bool Operation::DeserializeFromVector(const std::vector<uint8_t>& input) {
    eprosima::fastcdr::FastBuffer fast_buffer((char *) input.data(), input.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    uint32_t operationType; 
    cdr >> operationType >> m_num1 >> m_num2;
    m_operationType = (OPERATIONTYPE) operationType; 
    return true;
}

bool Result::SerializeToVector(std::vector<uint8_t>& output) const {
    output.resize(1024);
    eprosima::fastcdr::FastBuffer fast_buffer((char*) output.data(), output.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    cdr << m_result; 
    output.resize(cdr.getSerializedDataLength());

    return true;
}

bool Result::DeserializeFromVector(const std::vector<uint8_t>& input) {
    eprosima::fastcdr::FastBuffer fast_buffer((char *) input.data(), input.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    cdr >> m_result;
    
    return true;
}

