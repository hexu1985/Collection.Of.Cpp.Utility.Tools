#include "spdlog/spdlog.h"
#include "spdlog/cfg/argv.h" // for loading levels from argv

int main (int argc, char *argv[])
{
    // or from the command line:
    // ./example SPDLOG_LEVEL=info,mylogger=trace
    spdlog::cfg::load_argv_levels(argc, argv);
    spdlog::info("Welcome to spdlog!");
}
