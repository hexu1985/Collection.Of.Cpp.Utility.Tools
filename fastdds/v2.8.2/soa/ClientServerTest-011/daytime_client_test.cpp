#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <chrono>

#include <fastdds/dds/log/Log.hpp>

#include "DaytimeClient.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using namespace eprosima::fastdds::dds;
using std::cout;
using std::endl;
using std::atoi;

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
    //init_log();
    cout << "Starting client" << endl;

    DaytimeClient client;
    if (!client.init()) {
        cout << "client init failed" << endl;
        return -1;
    }

    while (!client.isReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << client.get_daytime() << std::endl;

    cout << "EVERYTHING STOPPED FINE" << endl;

    return 0;
}
