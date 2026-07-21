// Stopwatch support for spdlog

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"
void stopwatch_example()
{
    spdlog::stopwatch sw;    
    spdlog::debug("Elapsed {}", sw);
    spdlog::debug("Elapsed {:.3}", sw);       
}

int main()
{
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    stopwatch_example();
}
