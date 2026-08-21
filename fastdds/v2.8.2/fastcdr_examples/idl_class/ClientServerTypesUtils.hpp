#pragma once

#include "ClientServerTypes.h"
#include <fastcdr/FastBuffer.h>
#include <fastcdr/Cdr.h>
#include <vector>

namespace clientserver {

inline
bool SerializeToVector(const Operation& operation, std::vector<uint8_t>& output) {
    auto size = Operation::getCdrSerializedSize(operation);

    output.resize(size);
    eprosima::fastcdr::FastBuffer fast_buffer((char*) output.data(), output.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    operation.serialize(cdr);

    return true;
}

inline
bool DeserializeFromVector(Operation& operation, const std::vector<uint8_t>& input) {
    eprosima::fastcdr::FastBuffer fast_buffer((char *) input.data(), input.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    operation.deserialize(cdr);
    return true;
}

inline
bool SerializeToVector(const Result& result, std::vector<uint8_t>& output) {
    auto size = Result::getCdrSerializedSize(result);

    output.resize(size);
    eprosima::fastcdr::FastBuffer fast_buffer((char*) output.data(), output.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    result.serialize(cdr);

    return true;
}

inline
bool DeserializeFromVector(Result& result, const std::vector<uint8_t>& input) {
    eprosima::fastcdr::FastBuffer fast_buffer((char *) input.data(), input.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);

    result.deserialize(cdr);
    return true;
}

}   // namespace clientserver
