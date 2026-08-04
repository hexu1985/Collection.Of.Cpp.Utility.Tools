#include "ClientServerTypes.hpp"
#include <fastcdr/FastBuffer.h>
#include <cstring>

using namespace clientserver;

bool Operation::SerializeToString(std::string* output) const {
    char buffer[1024];
    eprosima::fastcdr::FastBuffer fast_buffer(buffer, sizeof(buffer));
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    cdr << (uint32_t) m_operationType << m_num1 << m_num2; 
    *output = std::string(
            reinterpret_cast<char*>(fast_buffer.getBuffer()),
            fast_buffer.getBufferSize()
            );

    return true;
}

bool Operation::ParseFromString(const std::string& data) {
    eprosima::fastcdr::FastBuffer fast_buffer((char *) data.data(), data.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    uint32_t operationType; 
    cdr >> operationType >> m_num1 >> m_num2;
    m_operationType = (clientserver::Operation::OPERATIONTYPE) operationType; 
    return true;
}

bool Result::SerializeToString(std::string* output) const {
    char buffer[1024];
    eprosima::fastcdr::FastBuffer fast_buffer(buffer, sizeof(buffer));
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    cdr << m_result; 
    *output = std::string(
            reinterpret_cast<char*>(fast_buffer.getBuffer()),
            fast_buffer.getBufferSize()
            );

    return true;
}

bool Result::ParseFromString(const std::string& data) {
    eprosima::fastcdr::FastBuffer fast_buffer((char *) data.data(), data.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    cdr >> m_result;
    
    return true;
}

