#include <thread>
#include <chrono>
#include <iostream>
#include "ring_buffer.hpp"

using namespace std::literals;

const size_t kPayloadSize = 16;

struct Frame {
    uint32_t index;
    uint8_t  data[kPayloadSize];
};

RingBuffer<Frame, 5> frames;

void producer() {
    for (size_t i = 0; i < 20; i++) {
        Frame& out = frames.push();
        out.index = i;
        std::fill_n(out.data, sizeof(out.data) - 1, 'a' + i);
        out.data[sizeof(out.data) - 1] = '\0';
        std::this_thread::sleep_for(100ms);
    }
}

void consumer() {
    for (size_t i = 0; i < 20; i++) {
        std::this_thread::sleep_for(200ms);
        if (!frames.has_data()) {
            continue;
        }
        const Frame& in = frames.pull();
        std::cout << "Frame " << in.index << ": " << in.data << std::endl;
    }
}

int main() {
    std::thread thr_produce, thr_consume;

    thr_produce = std::thread(consumer);
    thr_consume = std::thread(producer);

    thr_produce.join();
    thr_consume.join();
}
