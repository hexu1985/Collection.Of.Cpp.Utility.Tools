#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // 加载环境变量所需的头文件

int main(int, char *[])
{
    // 在程序启动时调用，它会读取SPDLOG_LEVEL环境变量
    spdlog::cfg::load_env_levels(); 

    // 现在，日志级别会根据环境变量被设置
    spdlog::trace("这是一条 trace 消息");
    spdlog::debug("这是一条 debug 消息");
    spdlog::info("这是一条 info 消息");
    spdlog::warn("这是一条 warn 消息");
    spdlog::error("这是一条 error 消息");
    spdlog::critical("这是一条 critical 消息");

    return 0;
}
