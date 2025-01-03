#include "frame_delay_checker.hpp"

FrameDelayChecker::FrameDelayChecker(std::chrono::milliseconds check_period_ms): check_period_ms_(check_period_ms) {
}

bool FrameDelayChecker::RegisterFrameDelayCheck(int index, const CheckConfig& config) {
    if (counters_.count(index)) {
        return false;
    }
    auto &counter = counters_[index]; 
    counter.expired_threshold_ms = config.expired_threshold_ms;
    counter.expired_callback = counter.expired_callback;
    return true;
}

bool FrameDelayChecker::IncreaseFrameCount(int index) {
    if (!counters_.count(index)) {
        return false;
    }
    counters_[index].current_frame_count++;
    return true;
}

void FrameDelayChecker::Start() {
}

void FrameDelayChecker::Stop() {
}
