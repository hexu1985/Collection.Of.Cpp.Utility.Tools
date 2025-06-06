#pragma once

#include <chrono>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <thread>

class FrameDelayChecker {
public:
    using Callback = std::function<void()>;

    struct CheckConfig {
        std::chrono::milliseconds expired_threshold_ms{};
        Callback expired_callback;
        Callback resumed_callback;
    };

    FrameDelayChecker(std::chrono::milliseconds check_period_ms);

    bool RegisterFrameDelayCheck(int index, const CheckConfig& config); 
    bool IncreaseFrameCount(int index); 

    void Start(); 
    void Stop();

    FrameDelayChecker(const FrameDelayChecker&) = delete;
    FrameDelayChecker& operator= (const FrameDelayChecker&) = delete;

private:
    struct Counter {
        std::atomic<uint32_t> current_frame_count{0};
        uint32_t latest_frame_count{0};
        std::chrono::steady_clock::time_point latest_frame_time{};
        std::chrono::milliseconds expired_threshold_ms{};
        bool expired = false;
        Callback expired_callback;
        Callback resumed_callback;
    };

    void InitialAllCounters();
    void CheckAndUpdateCounter(Counter& counter);
    void check_routine(); 

private:
    std::chrono::milliseconds check_period_ms_ = std::chrono::milliseconds{10};
    std::chrono::steady_clock::time_point current_time_{};
    std::unordered_map<int, Counter> counters_;
    std::thread check_thread_;
    std::atomic_bool done_{false};
};
