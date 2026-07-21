在 spdlog 中，同步日志是**默认且直接**的工作模式。使用同步日志时，`logger->info("...")` 这类调用会**阻塞当前线程**，直到日志消息被完全写入所有配置的输出目标（如控制台、文件）。这种方式逻辑简单，能保证日志的实时性和顺序性，适合日志量不大或对实时性要求高的场景。

下面是两种最常用同步日志方式的示例代码。

### 1. 使用全局默认日志器（最简单）

如果只是想在控制台快速打印日志，可以直接使用 spdlog 提供的全局函数，无需任何配置。

```cpp
#include <spdlog/spdlog.h>

int main() {
    // 默认只输出 info 及以上级别的日志 (info, warn, error, critical)
    spdlog::info("Welcome to spdlog!");
    spdlog::error("An error occurred with code: {}", 404); // 支持 {} 占位符

    // 如需显示 debug 和 trace 级别日志，需手动设置全局日志级别
    spdlog::set_level(spdlog::level::debug); 
    spdlog::debug("This is a debug message.");

    return 0;
}
```

### 2. 创建自定义同步日志器（推荐）

在实际项目中，更推荐创建独立的 `logger` 对象，以便灵活地控制输出目标（例如同时输出到控制台和文件）和日志格式。

```cpp
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
```

### 关键点总结

*   **创建方式**：默认的 `spdlog::info()` 函数是同步的。通过工厂函数如 `spdlog::stdout_color_mt` 创建的 `logger` 默认也是同步的。
*   **`_mt` 与 `_st` 后缀**：`_mt` 表示多线程安全（使用互斥锁），`_st` 表示单线程版本（性能略高，非线程安全）。
*   **性能特点**：同步日志在每次调用时都会执行 I/O 操作，高频日志可能会影响程序性能。对于高性能场景，可以考虑使用**异步日志**。
