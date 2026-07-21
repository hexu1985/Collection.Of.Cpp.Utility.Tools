下面提供几种使用 spdlog 异步日志的常见方式，你可以根据项目需求选择。

### 方式一：最简洁的异步日志（推荐）

这是最直接的方式，使用 `spdlog::create_async` 工厂方法，一行代码就能创建异步文件日志器。

```cpp
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
```

### 方式二：需要自定义线程池或拥有多个 Sink

当需要精细控制异步行为（如队列大小、后台线程数）或同时将日志输出到控制台和文件时，可以手动初始化线程池并创建 `async_logger`。

```cpp
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
```

### 关键配置与最佳实践

- **队列满时的策略 (`async_overflow_policy`)**：默认为 `block`（阻塞业务线程直到队列有空间），能保证消息不丢失。也可设置为 `overrun_oldest`（覆盖队列中最旧的消息）或 `discard_new`（丢弃新消息），适用于对延迟极度敏感的场景。
- **确保日志完整 (`flush`)**：程序退出前，务必调用 `logger->flush()` 或 `spdlog::drop_all()`，这会等待后台线程将队列中所有消息处理完毕。
