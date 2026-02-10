#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

void basic_logfile_example()
{
    try
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
        logger->info("Welcome to spdlog!");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}


int main()
{
    basic_logfile_example();
}
