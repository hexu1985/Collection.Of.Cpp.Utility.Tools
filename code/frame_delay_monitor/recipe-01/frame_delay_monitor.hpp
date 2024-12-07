#pragma once

#include <chrono>
#include <functional>
#include <array>
#include <atomic>

template <size_t N>
class FrameDelayMonitor {
public:
    using WarningFunction = std::function<void()>;

    struct Config {
        uint32_t expired_threshold_tick_number=10;
        WarningFunction warning_function;
    };

    FrameDelayMonitor(int max_counter_size, std::chrono::milliseconds ms_per_tick) {
    }

    bool RegisterFrameDelayWarning(const Config& config);

    void Start();
    void Stop();

private:
    struct Counter {
        std::atomic<uint32_t> current_count_{};
        uint32_t latest_count_=0;
        uint64_t latest_tick_=0;
        uint32_t expired_threshold_tick_number_=10;
        WarningFunction warning_function_;
    };

    std::chrono::milliseconds ms_per_tick_;
    uint64_t current_tick_;
    std::array<Counter, N> counters_;
};
