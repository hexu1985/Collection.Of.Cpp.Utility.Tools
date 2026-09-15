#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <chrono>

#include <fastdds/dds/log/Log.hpp>

#include "EprosimaClientTimed.h"
#include "EprosimaRpcUtility.hpp"

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

void test_client_timed(EprosimaClientTimed& client, int32_t num1, const std::string& op_str, int32_t num2, int32_t local_res, std::chrono::milliseconds timeout) {
    int32_t res = 0;
    OPERATIONTYPE op = to_operation_type(op_str);
    soa_on_dds::ErrorCode ec = client.calculate(op, num1, num2, &res, timeout);
    if (ec != soa_on_dds::SUCCESS) {
        cout << "client.calculate failed: " << EprosimaRpcUtility::error_code_to_string(ec) << endl;
        return;
    }
    cout << "calculate remote: " << num1 << op_str << num2 << "=" << res << endl;

    if (local_res == res) {
        cout << "remote result is correct!" << endl;
    } else {
        cout << "remote result is incorrect!" << endl;
    }
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
    //init_log();

    int32_t num1 = atoi(argv[1]);
    std::string op_str = argv[2];
    int32_t num2 = atoi(argv[3]);

    OPERATIONTYPE op = to_operation_type(op_str);
    int32_t local_res = calculate(num1, op, num2);
    cout << "calculate local: " << num1 << op_str << num2 << "=" << local_res << endl;

    cout << "Starting client" << endl;

    EprosimaClientTimed client;
    if (!client.init()) {
        cout << "client init failed" << endl;
        return -1;
    }

    while (!client.isReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "client.calculate" << std::endl;

    test_client_timed(client, num1, op_str, num2, local_res, std::chrono::milliseconds(2000));

    test_client_timed(client, num1, op_str, num2, local_res, std::chrono::milliseconds(12000));

    std::cout << "entry any key to exit: " << std::endl;
    std::cin.get();

    cout << "EVERYTHING STOPPED FINE" << endl;

    return 0;
}
