#include "spdlog/spdlog.h"

int main()
{
    // periodically flush all *registered* loggers every 3 seconds:
    // warning: only use if all your loggers are thread-safe ("_mt" loggers)
    spdlog::flush_every(std::chrono::seconds(3));
}
