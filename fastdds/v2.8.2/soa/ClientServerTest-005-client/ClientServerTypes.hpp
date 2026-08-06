#pragma once
#include <vector>
#include <cstdint>
#include <fastcdr/Cdr.h>

namespace clientserver {

class Operation
{
public:
    enum OPERATIONTYPE : uint32_t
    {
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
    };
    OPERATIONTYPE m_operationType;
    int32_t m_num1;
    int32_t m_num2;
    Operation()
        : m_operationType(ADDITION)
        , m_num1(0)
        , m_num2(0)
    {
    }

    ~Operation()
    {
    }

    bool SerializeToVector(std::vector<uint8_t>& output) const;
    bool DeserializeFromVector(const std::vector<uint8_t>& input);
};

class Result
{
public:
    int32_t m_result;
    Result()
        : m_result(0)
    {
    }

    ~Result()
    {
    }

    bool SerializeToVector(std::vector<uint8_t>& output) const;
    bool DeserializeFromVector(const std::vector<uint8_t>& input);
};

}   // namespace clientserver
