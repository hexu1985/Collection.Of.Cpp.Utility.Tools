#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // 彩色控制台输出
#include <spdlog/sinks/basic_file_sink.h>   // 基础文件输出

int main() {
    try {
        // 1. 创建输出目标 (sink)
        // 多线程安全版本的彩色控制台 sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        // 多线程安全版本的基础文件 sink，第二个参数为文件路径
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/sync_log.txt");

        // 2. 创建 logger，并关联多个 sink
        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("multi_sink_logger", sinks.begin(), sinks.end());

        // 3. 设置日志级别和输出格式
        logger->set_level(spdlog::level::trace); // 设置为最详细的级别
        // 格式: [时间] [日志器名] [线程ID] [日志级别] 消息内容
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%t] [%-8l] %v");

        // 4. 打印同步日志
        logger->trace("This is a trace message.");
        logger->debug("This is a debug message. Value: {}", 42);
        logger->info("Hello, {}!", "world");
        logger->warn("This is a warning.");
        logger->error("This is an error.");

        // 将自定义 logger 设置为全局默认，之后可以直接使用 spdlog::info() 
        // spdlog::set_default_logger(logger);
        // spdlog::info("Now using the custom logger as default.");

    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    return 0;
}
