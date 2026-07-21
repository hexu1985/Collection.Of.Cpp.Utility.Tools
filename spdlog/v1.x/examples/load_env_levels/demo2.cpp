#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    // 创建多个logger
    auto logger1 = spdlog::stdout_color_mt("logger1");
    auto logger2 = spdlog::stdout_color_mt("logger2");
    auto logger3 = spdlog::stdout_color_mt("logger3");
    
    // 从环境变量加载所有logger的级别
    spdlog::cfg::load_env_levels();
    
    logger1->info("Logger1 info");
    logger2->debug("Logger2 debug");
    logger3->trace("Logger3 trace");
    
    return 0;
}
