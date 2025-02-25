#include "frame_delay_checker.hpp"

FrameDelayChecker::FrameDelayChecker(std::chrono::milliseconds check_period_ms): check_period_ms_(check_period_ms) {
}

bool FrameDelayChecker::RegisterFrameDelayCheck(int index, const CheckConfig& config) {
    if (counters_.count(index)) {
        return false;
    }
    if (config.expired_threshold_ms <= check_period_ms_) {
        return false;
    }
    if (!config.expired_callback) {
        return false;
    }

    auto &counter = counters_[index]; 
    counter.expired_threshold_ms = config.expired_threshold_ms;
    counter.expired_callback = config.expired_callback;
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
    if (check_thread_.joinable()) {
        return;
    }
    done_ = false;
    check_thread_ = std::thread(&FrameDelayChecker::check_routine, this);
}

void FrameDelayChecker::Stop() {
    if (!check_thread_.joinable()) {
        return;
    }
    done_ = true;
    check_thread_.join();
}

void FrameDelayChecker::InitialAllCounters() {
    using std::chrono::steady_clock;
    current_time_ = steady_clock::now();
    for (auto& item : counters_) {
        auto& counter = item.second;
        counter.latest_frame_time = current_time_;
    }
}

void FrameDelayChecker::CheckAndUpdateCounter(Counter& counter) {
    if (counter.current_frame_count == 0) {
        return;
    }
    uint32_t current_frame_count = counter.current_frame_count;
    if (current_frame_count == counter.latest_frame_count) {
        auto duration = current_time_ - counter.latest_frame_time;
        if (duration > counter.expired_threshold_ms) {
            counter.expired_callback();
            return;
        }
    } else {
        counter.latest_frame_count = current_frame_count;
        counter.latest_frame_time = current_time_;
    }
}

void FrameDelayChecker::check_routine() {
    using std::chrono::steady_clock;
    using std::this_thread::sleep_until;

    InitialAllCounters();
    while (!done_) {
        current_time_ = steady_clock::now();
        for (auto& item : counters_) {
            auto& counter = item.second;
            CheckAndUpdateCounter(counter);
        }
        sleep_until(current_time_+check_period_ms_);
    }
}

