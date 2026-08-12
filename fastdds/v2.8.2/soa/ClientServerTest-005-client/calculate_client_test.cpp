#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <chrono>

#include <fastdds/dds/log/Log.hpp>

#include "EprosimaClient.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using namespace eprosima::fastdds::dds;
using std::cout;
using std::endl;
using std::atoi;
using namespace clientserver;

Operation::OPERATIONTYPE to_operation_type(const std::string& op_str) {
    if (op_str.empty()) {
        return Operation::ADDITION;
    }
    switch (op_str[0]) {
    case '+':
        return Operation::ADDITION;
    case '-':
        return Operation::SUBTRACTION;
    case '*':
        return Operation::MULTIPLICATION;
    case '/':
        return Operation::DIVISION;
    default:
        return Operation::ADDITION;
    }
}

int calculate(int32_t num1, Operation::OPERATIONTYPE op, int32_t num2) {
    switch (op) {
    case Operation::ADDITION:
        return num1 + num2;
    case Operation::SUBTRACTION:
        return num1 - num2;
    case Operation::MULTIPLICATION:
        return num1 * num2;
    case Operation::DIVISION:
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
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " num1 op num2" << endl;
        cout << "example: " << argv[0] << " 2 + 5 " << endl;
        return 1;
    }
//    init_log();

    int32_t num1 = atoi(argv[1]);
    std::string op_str = argv[2];
    int32_t num2 = atoi(argv[3]);

    Operation::OPERATIONTYPE op = to_operation_type(op_str);
    int32_t local_res = calculate(num1, op, num2);
    cout << "calculate local: " << num1 << op_str << num2 << "=" << local_res << endl;

    cout << "Starting client" << endl;

    EprosimaClient client;
    client.init();

    while (!client.isReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "client.calculate" << std::endl;

    int32_t res = 0;
    soa_on_dds::ErrorCode ec = client.calculate(op, num1, num2, &res);
    if (ec != soa_on_dds::SUCCESS) {
        //cout << "client.calculate failed: " << to_string(res_type) << endl;
        return 1;
    }
    cout << "calculate remote: " << num1 << op_str << num2 << "=" << res << endl;

    if (local_res == res) {
        cout << "remote result is correct!" << endl;
    } else {
        cout << "remote result is incorrect!" << endl;
    }

    cout << "EVERYTHING STOPPED FINE" << endl;

    return 0;
}
