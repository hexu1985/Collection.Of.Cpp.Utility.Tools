#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

int main() {
    // 创建一个logger
    auto logger = spdlog::stdout_color_mt("console");

    // 从环境变量 SPDLOG_LEVEL 加载日志级别
    spdlog::load_env_levels();

    // 设置默认logger
    spdlog::set_default_logger(logger);

    // 各种级别的日志输出
    SPDLOG_TRACE("This is a trace message");
    SPDLOG_DEBUG("This is a debug message");
    SPDLOG_INFO("This is an info message");
    SPDLOG_WARN("This is a warning message");
    SPDLOG_ERROR("This is an error message");
    SPDLOG_CRITICAL("This is a critical message");

    return 0;
}
