#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>

#include "EprosimaServer.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using std::cout;
using std::endl;

int main(
        int argc,
        char** argv)
{
    cout << "Starting server" << endl;

    EprosimaServer server;
    server.init();
    server.serve();

    cout << "EVERYTHING STOPPED FINE" << endl;

    return 0;
}
