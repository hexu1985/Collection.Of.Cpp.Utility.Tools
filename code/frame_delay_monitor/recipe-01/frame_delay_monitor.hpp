#pragma once

#include <chrono>
#include <functional>
#include <array>
#include <atomic>
#include <thread>

template <size_t N>
class FrameDelayMonitor {
public:
    using WarningFunction = std::function<void()>;

    struct Config {
        uint32_t expired_threshold_tick_number=10;
        WarningFunction warning_function;
    };

    FrameDelayMonitor(std::chrono::milliseconds ms_per_tick): ms_per_tick_(ms_per_tick) {
    }

    bool RegisterFrameDelayWarning(size_t index, const Config& config) {
        if (index > counters_.size() || !config.warning_function) {
            return false;
        }
        auto& counter = counters_[index];
        counter.current_count = 0;
        counter.latest_count = 0;
        counter.latest_tick = 0;
        counter.expired_threshold_tick_number = config.expired_threshold_tick_number;
        counter.warning_function = config.warning_function;

        return true;
    }

    bool IncreaseFrameCount(size_t index) {
        if (index > counters_.size()) {
            return false;
        }
        auto& counter = counters_[index];
        counter.current_count++;

        return true;
    }

    void Start() {
        if (monitor_thread_.joinable()) {
            return;
        }
        monitor_thread_ = std::thread(&FrameDelayMonitor::MonitorLoop, this); 
    }

    void Stop();

private:
    void MonitorLoop() {
    }

private:
    struct Counter {
        std::atomic<uint32_t> current_count{0};
        uint32_t latest_count=0;
        uint64_t latest_tick=0;
        uint32_t expired_threshold_tick_number=10;
        WarningFunction warning_function;
    };

    std::chrono::milliseconds ms_per_tick_ = std::chrono::milliseconds{10};
    uint64_t current_tick_ = 0;
    std::array<Counter, N> counters_;
    std::thread monitor_thread_;
    std::atomic_bool done_ = false;
};
