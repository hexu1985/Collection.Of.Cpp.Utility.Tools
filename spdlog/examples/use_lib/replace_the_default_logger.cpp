#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

void replace_default_logger_example()
{
    auto new_logger = spdlog::basic_logger_mt("new_default_logger", "logs/new-default-log.txt", true);
    spdlog::set_default_logger(new_logger);
    spdlog::info("new logger log message");
}

int main()
{
    replace_default_logger_example();
}
