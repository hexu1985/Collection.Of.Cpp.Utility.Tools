#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void err_handler_example()
{
    // can be set globally or per logger(logger->set_error_handler(..))
    spdlog::set_error_handler([](const std::string &msg) { spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); });
    spdlog::get("console")->info("some invalid message to trigger an error {}{}{}{}", 3);
}

int main()
{
    auto logger = spdlog::stdout_color_mt("console");
    err_handler_example();
}
