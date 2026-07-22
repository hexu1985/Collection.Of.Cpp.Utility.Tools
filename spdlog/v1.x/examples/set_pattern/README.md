`set_pattern` 是 spdlog 里最核心的格式化方法，它通过一个模式字符串，灵活地控制每一条日志的输出内容。

你可以为所有日志器统一设置格式，也可以只为某一个特定的日志器定制。

```cpp
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
```

### 📝 常用格式标记符 (Pattern Flags)

你可以通过组合下面的标记符来构建自己的格式字符串。

| 标记符 | 含义 | 示例输出 |
| :--- | :--- | :--- |
| **`%v`** | 实际的日志消息文本 | `"Hello, world!"` |
| **`%l`** | 日志级别（完整名称） | `info`, `warning`, `error` |
| **`%L`** | 日志级别（简短名称） | `I`, `W`, `E` |
| **`%n`** | 当前日志器的名称 | `my_console` |
| **`%t`** | 线程 ID | `1234` |
| **`%P`** | 进程 ID | `5678` |
| **`%Y`** | 四位数的年份 | `2026` |
| **`%m`** | 月份（两位数） | `07` |
| **`%d`** | 日期（两位数） | `21` |
| **`%H`** | 小时（24小时制，两位数） | `14` |
| **`%M`** | 分钟（两位数） | `05` |
| **`%S`** | 秒（两位数） | `30` |
| **`%e`** | 毫秒（三位数） | `123` |
| **`%f`** | 微秒（六位数） | `456789` |
| **`%F`** | 纳秒（九位数） | `123456789` |
| **`%@`** | 源文件路径和行号（需配合宏使用） | `main.cpp:42` |
| **`%!`** | 函数名（需配合宏使用） | `main` |
| **`%^`** | 颜色范围开始 | (启用终端颜色) |
| **`%$`** | 颜色范围结束 | (终止颜色区域) |
| **`%+`** | spdlog 的默认格式 | `[2014-10-31 23:46:59.678] [info] ...` |

> **请注意**：标记符 `%@`（文件名与行号）和 `%!`（函数名）需要配合 `SPDLOG_INFO` 这类宏来记录日志才会生效，直接调用 `logger->info()` 不会输出这些信息。

要输出源文件路径、行号和函数名，不能直接调用 `logger->info()`，必须使用 spdlog 提供的**日志宏**（如 `SPDLOG_INFO`、`SPDLOG_DEBUG` 等），因为这些宏会捕获调用位置的 `__FILE__`、`__LINE__` 和 `__FUNCTION__` 信息。

下面给一个完整的可运行示例：

```cpp
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
```

### 📋 编译和运行

编译时需要启用 C++11 或更高版本，并链接 spdlog 库（如果使用头文件版本，只需包含头文件路径）：

```bash
g++ -std=c++11 -o test test.cpp -I/path/to/spdlog/include
```

运行后输出效果类似：

```
[2026-07-22 14:23:45.123] [info] [test.cpp:20] [main] 程序启动
[2026-07-22 14:23:45.124] [info] [test.cpp:10] [doSomething] 正在执行某个操作
[2026-07-22 14:23:45.124] [warning] [test.cpp:11] [doSomething] 警告：参数可能无效
[2026-07-22 14:23:45.124] [debug] [test.cpp:28] [main] 这条 debug 日志现在可以看到
```

### ⚠️ 重要注意事项

| 要点 | 说明 |
| :--- | :--- |
| **必须使用宏** | `SPDLOG_INFO`、`SPDLOG_WARN`、`SPDLOG_ERROR`、`SPDLOG_DEBUG` 等，而不是 `logger->info()` |
| **`%@` 和 `%!` 协同** | `%@` 输出 `文件名:行号`，`%!` 输出函数名，两者配合才能获得完整信息 |
| **默认日志器** | 如果使用 `SPDLOG_INFO` 等宏，它们会使用默认日志器，需用 `spdlog::set_default_logger()` 设置 |
| **调试信息可见** | `SPDLOG_DEBUG` 默认不输出，需要调用 `spdlog::set_level(spdlog::level::debug)` |
| **性能考量** | 使用宏会多一次函数调用，但对性能影响微乎其微，可以放心使用 |

### 🎨 进阶：让格式更紧凑

如果你希望输出更简洁，可以这样设置：

```cpp
// 仅显示文件名（不含路径）、行号和函数名
spdlog::set_pattern("[%l] %@ %! : %v");
```

输出效果：
```
[info] test.cpp:20 main : 程序启动
[warning] test.cpp:11 doSomething : 警告：参数可能无效
```

这样就能清晰地定位每条日志来自代码的哪个位置了。

如果不想使用默认日志器，但又想获取源文件路径、行号和函数名，可以使用 `SPDLOG_LOGGER_*` 系列的宏，它们允许你指定自定义日志器，同时自动捕获位置信息。

## 完整解决方案

```cpp
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
```

## 输出效果

```
[2026-07-22 14:30:15.123] [info] [test.cpp:25] [processData] 处理数据，值 = 50
[2026-07-22 14:30:15.124] [warning] [test.cpp:28] [processData] 检测到负数: -5
[2026-07-22 14:30:15.124] [error] [test.cpp:31] [processData] 数值超出范围: 150
[2026-07-22 14:30:15.124] [info] [test.cpp:40] [doSomething] MyClass::doSomething 被调用
[2026-07-22 14:30:15.124] [debug] [test.cpp:43] [doSomething] 成员变量值: 42
```

## 核心要点

### 1. **使用正确的宏**

| 宏 | 用途 | 位置信息 |
| :--- | :--- | :--- |
| `SPDLOG_LOGGER_INFO(logger, ...)` | 指定日志器记录 info 级别 | ✅ 自动捕获 |
| `SPDLOG_LOGGER_WARN(logger, ...)` | 指定日志器记录 warning 级别 | ✅ 自动捕获 |
| `SPDLOG_LOGGER_ERROR(logger, ...)` | 指定日志器记录 error 级别 | ✅ 自动捕获 |
| `SPDLOG_LOGGER_DEBUG(logger, ...)` | 指定日志器记录 debug 级别 | ✅ 自动捕获 |
| `logger->info(...)` | 直接调用方法 | ❌ 无法捕获 |

### 2. **设置正确的格式**

```cpp
// 包含文件名、行号和函数名
logger->set_pattern("[%@] [%!] %v");
// 输出示例: [main.cpp:20] [main] 程序启动

// 更完整的格式
logger->set_pattern("%Y-%m-%d %H:%M:%S [%l] %@ %! : %v");
// 输出示例: 2026-07-22 14:30:15 [info] main.cpp:20 main : 程序启动
```

## 总结

| 方案 | 是否捕获位置信息 | 是否使用默认日志器 | 推荐度 |
| :--- | :--- | :--- | :--- |
| `SPDLOG_LOGGER_INFO` + 自定义日志器 | ✅ 是 | ❌ 否 | ⭐⭐⭐⭐⭐ 强烈推荐 |
| `SPDLOG_INFO` + 默认日志器 | ✅ 是 | ✅ 是 | ⭐⭐⭐ 可选 |
| `logger->info()` + 自定义日志器 | ❌ 否 | ❌ 否 | ⭐⭐ 不推荐（无位置信息） |

**关键点**：使用 `SPDLOG_LOGGER_*` 系列的宏，既能指定自定义日志器，又能自动捕获源文件、行号和函数名，完美解决了你的需求。
