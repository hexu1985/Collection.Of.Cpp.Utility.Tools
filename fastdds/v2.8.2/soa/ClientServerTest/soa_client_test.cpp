#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>

#include "EprosimaClient.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using std::cout;
using std::endl;
using std::atoi;
using clientserver::Operation;

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

int main(
        int argc,
        char** argv)
{
    cout << "Starting " << endl;

    if (argc != 4) {
        cout << "Usage: " << argv[0] << " num1 op num2" << endl;
        cout << "example: " << argv[0] << " 2 + 5 " << endl;
        return 1;
    }

    int32_t num1 = atoi(argv[1]);
    std::string op_str = argv[2];
    int32_t num2 = atoi(argv[3]);

    Operation::OPERATIONTYPE op = to_operation_type(op_str);
    int32_t result = calculate(num1, op, num2);
    cout << "calculate local: " << num1 << op_str << num2 << "=" << result << endl;

    EprosimaClient client;
    client.init();


    cout << "EVERYTHING STOPPED FINE" << endl;

    return 0;
}
