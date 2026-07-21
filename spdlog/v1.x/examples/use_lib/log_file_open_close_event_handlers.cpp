#include <cstdio>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

// You can get callbacks from spdlog before/after a log file has been opened or closed. 
// This is useful for cleanup procedures or for adding something to the start/end of the log file.
void file_events_example()
{
    // pass the spdlog::file_event_handlers to file sinks for open/close log file notifications
    spdlog::file_event_handlers handlers;
    handlers.before_open = [](spdlog::filename_t filename) { spdlog::info("Before opening {}", filename); };
    handlers.after_open = [](spdlog::filename_t filename, std::FILE *fstream) { fputs("After opening\n", fstream); };
    handlers.before_close = [](spdlog::filename_t filename, std::FILE *fstream) { fputs("Before closing\n", fstream); };
    handlers.after_close = [](spdlog::filename_t filename) { spdlog::info("After closing {}", filename); };
    auto my_logger = spdlog::basic_logger_st("some_logger", "logs/events-sample.txt", true, handlers);        
}

int main()
{
    file_events_example();
}
