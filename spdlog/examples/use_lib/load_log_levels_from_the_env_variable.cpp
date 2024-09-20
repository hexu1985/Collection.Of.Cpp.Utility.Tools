#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

int main (int argc, char *argv[])
{
    spdlog::cfg::load_env_levels();
    // export SPDLOG_LEVEL=info,mylogger=trace
    // ./example
    spdlog::info("Welcome to spdlog!");
}
