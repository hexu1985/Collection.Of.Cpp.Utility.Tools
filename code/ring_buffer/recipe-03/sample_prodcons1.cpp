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
    Frame frame;
    for (size_t i = 0; i < 20; i++) {
        frame.index = i;
        std::fill_n(frame.data, sizeof(frame.data) - 1, 'a' + i);
        frame.data[sizeof(frame.data) - 1] = '\0';
        frames.put(frame);
        std::this_thread::sleep_for(100ms);
    }
}

void consumer() {
    Frame frame;
    for (size_t i = 0; i < 20; i++) {
        std::this_thread::sleep_for(200ms);
        if (!frames.has_data()) {
            continue;
        }
        frames.get(frame);
        std::cout << "Frame " << frame.index << ": " << frame.data << std::endl;
    }
}

int main() {
    std::thread thr_produce, thr_consume;

    thr_produce = std::thread(consumer);
    thr_consume = std::thread(producer);

    thr_produce.join();
    thr_consume.join();
}
