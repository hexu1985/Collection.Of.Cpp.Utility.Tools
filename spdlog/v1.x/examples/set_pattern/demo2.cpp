#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// 自定义函数，用来演示函数名的输出
void doSomething() {
    // 使用宏记录日志，这样会捕获源文件、行号和函数名
    SPDLOG_INFO("正在执行某个操作");
    SPDLOG_WARN("警告：参数可能无效");
}

int main() {
    // 1. 设置输出格式
    // %@ = 源文件:行号   %! = 函数名   %v = 日志消息
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%@] [%!] %v");

    // 2. 创建日志器（控制台输出便于观察）
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);  // 设为默认日志器

    // 3. 在 main 函数中记录日志
    SPDLOG_INFO("程序启动");   // 会显示 main.cpp:行号 和 main 函数名
    SPDLOG_DEBUG("调试信息");  // 默认情况下 debug 级别可能不输出，需设置级别

    // 4. 在自定义函数中记录日志
    doSomething();

    // 5. 设置日志级别以确保 debug 信息也能输出
    spdlog::set_level(spdlog::level::debug);
    SPDLOG_DEBUG("这条 debug 日志现在可以看到");

    // 6. 刷新并清理
    spdlog::drop_all();
    return 0;
}
