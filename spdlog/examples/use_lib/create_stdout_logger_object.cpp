#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void stdout_example()
{
    // create a color multi-threaded logger
    auto console = spdlog::stdout_color_mt("console");    
    auto err_logger = spdlog::stderr_color_mt("stderr");    
    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");
}

int main()
{
    stdout_example();
}
