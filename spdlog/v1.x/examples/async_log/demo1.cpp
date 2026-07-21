#include <spdlog/spdlog.h>
#include <spdlog/async.h>                  // 异步日志头文件
#include <spdlog/sinks/basic_file_sink.h>  // 基础文件输出

int main() {
    // 创建一个异步文件日志器
    // 参数: 日志器名称, 日志文件名
    auto async_file_logger = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(
        "async_logger", "logs/async_log.txt"
    );

    // 设置日志级别和格式（可选）
    async_file_logger->set_level(spdlog::level::info);
    async_file_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    // 业务线程中直接记录日志，主线程不会被磁盘 I/O 阻塞
    async_file_logger->info("Hello, this is an async log message!");
    async_file_logger->warn("Warning with arguments: {}", 42);

    // 确保程序退出前所有日志都已写入磁盘
    async_file_logger->flush();
    spdlog::drop_all();

    return 0;
}
