#include "frame_delay_checker.hpp"

FrameDelayChecker::FrameDelayChecker(std::chrono::milliseconds check_period_ms): check_period_ms_(check_period_ms) {
}

bool FrameDelayChecker::RegisterFrameDelayCheck(int index, const CheckConfig& config) {
    return true;
}

bool FrameDelayChecker::IncreaseFrameCount(int index) {
    return true;
}

void FrameDelayChecker::Start() {
}

void FrameDelayChecker::Stop() {
}
