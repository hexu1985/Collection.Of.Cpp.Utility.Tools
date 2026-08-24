#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <chrono>

#include <fastdds/dds/log/Log.hpp>

#include "EprosimaClientTest.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using namespace eprosima::fastdds::dds;
using std::cout;
using std::endl;
using std::atoi;
using namespace clientserver;

OPERATIONTYPE to_operation_type(const std::string& op_str) {
    if (op_str.empty()) {
        return ADDITION;
    }
    switch (op_str[0]) {
    case '+':
        return ADDITION;
    case '-':
        return SUBTRACTION;
    case '*':
        return MULTIPLICATION;
    case '/':
        return DIVISION;
    default:
        return ADDITION;
    }
}

int calculate(int32_t num1, OPERATIONTYPE op, int32_t num2) {
    switch (op) {
    case ADDITION:
        return num1 + num2;
    case SUBTRACTION:
        return num1 - num2;
    case MULTIPLICATION:
        return num1 * num2;
    case DIVISION:
        return num1 / num2;
    default:
        return 0;
    }
}

void init_log() {
    // log info
    //Log::SetVerbosity(Log::Kind::Info);
    Log::SetVerbosity(Log::Kind::Warning);
    //Log::SetCategoryFilter(std::regex("(RTPS_|SECURITY_)"));  // 可选: 设置类别过滤器

    // 启用文件名和行号显示
    Log::ReportFilenames(true);  // 显示文件名
    Log::ReportFunctions(true);  // 显示函数名（可选）
}

int main(
        int argc,
        char** argv)
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " count" << endl;
        cout << "example: " << argv[0] << " 1000 " << endl;
        return 1;
    }
    //init_log();

    int32_t samples = atoi(argv[1]);

    EprosimaClientTest clienttest;
    double result = clienttest.run(samples);
    if (result > 0)
    {
        cout << "Mean Time of " << samples << " samples: " << result << " us" << endl;
    }
    else
    {
        cout << "Some problem with the test " << endl;
    }

    return 0;
}
