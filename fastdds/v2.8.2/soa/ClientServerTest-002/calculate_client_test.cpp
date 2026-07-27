#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <chrono>

#include "EprosimaClient.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
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

const char* to_string(RESULTTYPE res_type) {
    switch (res_type) {
    case GOOD_RESULT:
        return "GOOD_RESULT";
    case ERROR_RESULT:
        return "ERROR_RESULT";
    case SERVER_NOT_READY:
        return "SERVER_NOT_READY";
    default:
        return "UNKNOWN";
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

    int32_t num1 = atoi(argv[1]);
    std::string op_str = argv[2];
    int32_t num2 = atoi(argv[3]);

    OPERATIONTYPE op = to_operation_type(op_str);
    int32_t local_res = calculate(num1, op, num2);
    cout << "calculate local: " << num1 << op_str << num2 << "=" << local_res << endl;

    cout << "Starting client" << endl;

    EprosimaClient client;
    client.init();

    while (!client.isReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int32_t res = 0;
    RESULTTYPE res_type = client.calculate(op, num1, num2, &res);
    if (res_type != GOOD_RESULT) {
        cout << "client.calculate failed: " << to_string(res_type) << endl;
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
