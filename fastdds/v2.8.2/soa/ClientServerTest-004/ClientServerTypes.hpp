#pragma once
#include <string>
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

    bool SerializeToString(std::string* output) const;
    bool ParseFromString(const std::string& data);
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

    bool SerializeToString(std::string* output) const;
    bool ParseFromString(const std::string& data);
};

}   // namespace clientserver
