#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    // 1. 为所有日志器设置全局格式
    // 格式: [日期 时间.毫秒] [日志级别(带颜色)] 消息内容
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    spdlog::info("这是一条全局格式的日志");

    // 2. 为特定日志器设置独立格式
    auto console_logger = spdlog::stdout_color_mt("my_console");
    console_logger->set_pattern(">>>>>> %H:%M:%S %v <<<<<<");
    console_logger->info("这条日志使用独立格式");

    // 3. 恢复为 spdlog 的默认格式 ("%+")
    spdlog::set_pattern("%+");
    spdlog::info("日志格式已恢复为默认");

    return 0;
}
