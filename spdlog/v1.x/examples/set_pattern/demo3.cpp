#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// 为了方便，可以定义简短的宏别名
#define LOG_INFO(logger, ...)    SPDLOG_LOGGER_INFO(logger, __VA_ARGS__)
#define LOG_WARN(logger, ...)    SPDLOG_LOGGER_WARN(logger, __VA_ARGS__)
#define LOG_ERROR(logger, ...)   SPDLOG_LOGGER_ERROR(logger, __VA_ARGS__)
#define LOG_DEBUG(logger, ...)   SPDLOG_LOGGER_DEBUG(logger, __VA_ARGS__)

// 或者更简洁的方式（如果需要追踪更多信息）
#define LOG_TRACE(logger, ...)   SPDLOG_LOGGER_TRACE(logger, __VA_ARGS__)

void processData(std::shared_ptr<spdlog::logger> logger, int value) {
    // 使用自定义日志器，同时自动捕获文件名、行号和函数名
    LOG_INFO(logger, "处理数据，值 = {}", value);
    
    if (value < 0) {
        LOG_WARN(logger, "检测到负数: {}", value);
    }
    
    if (value > 100) {
        LOG_ERROR(logger, "数值超出范围: {}", value);
    }
}

class MyClass {
public:
    void doSomething(std::shared_ptr<spdlog::logger> logger) {
        LOG_INFO(logger, "MyClass::doSomething 被调用");
        
        // 类成员函数中也能正确捕获函数名
        LOG_DEBUG(logger, "成员变量值: {}", member_value);
    }
    
private:
    int member_value = 42;
};

int main() {
    // 1. 创建多个独立的日志器，都不设为默认日志器
    auto console_logger = spdlog::stdout_color_mt("console_logger");
    auto file_logger = spdlog::basic_logger_mt("file_logger", "output.log");
    
    // 2. 设置格式，包含源文件、行号和函数名
    console_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%@] [%!] %v");
    file_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%@] [%!] %v");
    
    // 设置日志级别
    console_logger->set_level(spdlog::level::debug);
    file_logger->set_level(spdlog::level::debug);
    
    // 3. 使用自定义日志器记录日志，自动捕获位置信息
    LOG_INFO(console_logger, "程序启动");
    LOG_INFO(file_logger, "程序启动（写入文件）");
    
    // 4. 在函数中使用
    processData(console_logger, 50);
    processData(file_logger, -5);
    processData(console_logger, 150);
    
    // 5. 在类中使用
    MyClass obj;
    obj.doSomething(console_logger);
    
    // 6. 直接调用方法也可以（但不会捕获位置信息）
    // 注意：这种方式不会显示源文件和行号！
    console_logger->info("直接调用，不会显示位置信息");
    
    // 7. 清理
    spdlog::drop_all();
    return 0;
}
