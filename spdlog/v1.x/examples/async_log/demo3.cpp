#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <thread>
#include <chrono>

int main() {
    // ============ 方式1：使用默认异步工厂 ============
    // 初始化线程池（队列大小8192，1个后台线程）
    spdlog::init_thread_pool(8192, 1);
    
    // 创建异步日志器
    auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        "async_logger", "logs/async_log.txt"
    );
    
    // 设置为默认日志器（可选）
    spdlog::set_default_logger(async_logger);
    
    // 使用异步日志
    async_logger->info("异步日志消息 1");
    spdlog::info("使用默认异步日志器");
    
    // ============ 方式2：创建多个sink的异步日志器 ============
    // 创建控制台sink和文件sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "logs/rotating_async.txt", 1024 * 1024 * 5, 3  // 5MB, 3个文件
    );
    file_sink->set_level(spdlog::level::info);
    
    // 组合多个sink
    spdlog::sinks_init_list sink_list = { console_sink, file_sink };
    
    // 创建异步日志器
    auto multi_sink_logger = std::make_shared<spdlog::async_logger>(
        "multi_sink", sink_list, spdlog::thread_pool(), 
        spdlog::async_overflow_policy::block  // 队列满时阻塞
    );
    
    multi_sink_logger->set_level(spdlog::level::trace);
    spdlog::register_logger(multi_sink_logger);
    
    // ============ 方式3：自定义线程池 ============
    auto custom_tp = std::make_shared<spdlog::details::thread_pool>(
        32768,   // 队列大小
        2        // 线程数
    );
    
    auto custom_async = std::make_shared<spdlog::async_logger>(
        "custom_async",
        file_sink,
        custom_tp,
        spdlog::async_overflow_policy::overrun_oldest  // 队列满时丢弃旧消息
    );
    
    spdlog::register_logger(custom_async);
    
    // ============ 使用示例 ============
    // 基本日志打印
    async_logger->trace("这是trace级别日志");
    async_logger->debug("这是debug级别日志");
    async_logger->info("这是info级别日志");
    async_logger->warn("这是warn级别日志");
    async_logger->error("这是error级别日志");
    async_logger->critical("这是critical级别日志");
    
    // 格式化日志
    async_logger->info("用户{}的年龄是{}", "张三", 25);
    async_logger->info("PI值约等于 {:.3f}", 3.14159);
    
    // 多线程测试
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&async_logger, i]() {
            for (int j = 0; j < 100; ++j) {
                async_logger->info("线程{} - 第{}条消息", i, j);
            }
        });
    }
    
    // 使用多sink日志器
    multi_sink_logger->info("这条消息会同时输出到控制台和文件");
    multi_sink_logger->trace("trace级别只输出到控制台（根据sink级别设置）");
    
    // 自定义线程池日志器
    custom_async->info("使用自定义线程池的异步日志");
    
    // 等待线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    // ============ 高级功能 ============
    // 手动刷新（确保所有异步日志写完）
    async_logger->flush();
    multi_sink_logger->flush();
    custom_async->flush();
    
    // 设置刷新策略
    async_logger->flush_on(spdlog::level::err);  // 遇到error及以上级别自动刷新
    
    // 获取当前日志级别
    spdlog::level::level_enum current_level = async_logger->level();
    
    // 暂停一小段时间，确保异步日志写入完成
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return 0;
}
