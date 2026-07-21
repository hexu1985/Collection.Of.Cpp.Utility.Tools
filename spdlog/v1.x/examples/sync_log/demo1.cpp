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
