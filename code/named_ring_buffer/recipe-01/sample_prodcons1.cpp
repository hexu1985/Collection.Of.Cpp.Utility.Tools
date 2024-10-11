#include <thread>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include "named_ring_buffer.hpp"

using namespace std::literals;

const size_t kPayloadSize = 16;
const char* kRingBufferName = "/sample_ring_buffer";

struct Frame {
    uint32_t index;
    uint8_t  data[kPayloadSize];
};

NamedRingBuffer<Frame, 5> frames{kRingBufferName};

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
        frames.get(frame);
        std::cout << "Frame " << frame.index << ": " << frame.data << std::endl;
    }
}

int main() {
    if (fork()) {
        consumer();
    } else {
        producer();
    }
}
