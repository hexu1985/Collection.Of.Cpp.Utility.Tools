#include "spdlog/spdlog.h"

int main()
{
    // Debug messages can be stored in a ring buffer instead of being logged immediately.
    // This is useful to display debug logs only when needed (e.g. when an error happens).
    // When needed, call dump_backtrace() to dump them to your log.

    spdlog::enable_backtrace(32); // Store the latest 32 messages in a buffer. 
    // or my_logger->enable_backtrace(32)..
    for(int i = 0; i < 100; i++)
    {
        spdlog::debug("Backtrace message {}", i); // not logged yet..
    }
    // e.g. if some error happened:
    spdlog::dump_backtrace(); // log them now! show the last 32 messages
    // or my_logger->dump_backtrace(32)..
}
