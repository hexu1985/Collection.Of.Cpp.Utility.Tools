使用 `spdlog::cfg::load_env_levels()` 可以很方便地从环境变量 `SPDLOG_LEVEL` 中加载日志级别配置，这在需要动态控制日志输出的场景下非常实用。下面是一个完整的示例。

### 核心示例代码

首先，你需要包含 `spdlog/cfg/env.h` 头文件，然后在程序初始化阶段调用 `spdlog::cfg::load_env_levels()`。

```cpp
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // 加载环境变量所需的头文件

int main(int, char *[])
{
    // 在程序启动时调用，它会读取SPDLOG_LEVEL环境变量
    spdlog::cfg::load_env_levels(); 

    // 现在，日志级别会根据环境变量被设置
    spdlog::trace("这是一条 trace 消息");
    spdlog::debug("这是一条 debug 消息");
    spdlog::info("这是一条 info 消息");
    spdlog::warn("这是一条 warn 消息");
    spdlog::error("这是一条 error 消息");
    spdlog::critical("这是一条 critical 消息");

    return 0;
}
```

> **说明**：`load_env_levels()` 函数会查找名为 `SPDLOG_LEVEL` 的环境变量，并根据其值设置所有已注册 logger 的日志级别。如果在环境变量中找不到该设置，则所有 logger 将使用默认级别（info）。

### 如何使用环境变量

你可以在运行程序前，通过命令行设置 `SPDLOG_LEVEL` 环境变量来控制日志输出。

#### 1. 设置全局日志级别

设置全局级别为 `debug`，这样所有 `debug` 及以上级别的日志都会输出。

```bash
# Linux / macOS
SPDLOG_LEVEL=debug ./your_program

# Windows (PowerShell)
$env:SPDLOG_LEVEL="debug"; .\your_program.exe
```

#### 2. 为特定 Logger 设置不同级别

这个功能特别适合在复杂的系统中调试特定的模块。你可以为默认 logger 设置一个级别，同时为其他命名的 logger 设置不同的级别。语法是用逗号分隔多个规则。

假设你的代码中有一个名为 `mylogger` 的 logger：
```cpp
auto mylogger = spdlog::stdout_color_mt("mylogger");
```

然后，你可以这样设置环境变量，让全局级别为 `info`，但让 `mylogger` 的级别为 `trace`：
```bash
SPDLOG_LEVEL=info,mylogger=trace ./your_program
```

#### 3. 精细化控制：关闭大部分日志，只保留特定模块

使用 `off` 可以关闭所有日志，然后只为需要调试的模块单独开启。

例如，只打开 `mylogger` 的 `debug` 级别日志，其他所有 logger 的日志都关闭：
```bash
SPDLOG_LEVEL="off,mylogger=debug" ./your_program
```

### 总结

`spdlog::cfg::load_env_levels()` 提供了一种零成本、运行时动态调整日志级别的机制，非常适合在生产环境中快速诊断问题，无需重新编译或修改配置文件。
