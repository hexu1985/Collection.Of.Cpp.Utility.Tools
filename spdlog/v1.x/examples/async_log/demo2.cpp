#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <vector>

int main() {
    // 1. 初始化线程池: 队列大小(8192), 后台线程数(1)
    spdlog::init_thread_pool(8192, 1);

    // 2. 准备多个 Sink: 一个输出到彩色控制台，一个输出到按大小轮转的文件
    std::vector<spdlog::sink_ptr> sinks;
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "logs/rotating_log.txt",  // 文件名
        1024 * 1024 * 5,          // 单个文件最大大小 (5MB)
        3                         // 最多保留的文件数量
    );
    sinks.push_back(console_sink);
    sinks.push_back(file_sink);

    // 3. 创建异步日志器
    auto logger = std::make_shared<spdlog::async_logger>(
        "multi_sink_logger",                         // 日志器名称
        sinks.begin(), sinks.end(),                  // Sink 迭代器
        spdlog::thread_pool(),                       // 使用全局线程池
        spdlog::async_overflow_policy::block         // 队列满时的策略: 阻塞
    );
    spdlog::register_logger(logger);                 // 注册到全局管理

    // 4. 使用日志器
    logger->info("This log goes to both console and file asynchronously.");

    // 5. 确保退出前刷新所有日志
    spdlog::drop_all();
    return 0;
}
