#include <iostream>

#include "threadsafe_ring_buffer.hpp"

struct Frame {
    uint32_t index;
    uint8_t  data[1024];
};

int main() {
    ThreadsafeRingBuffer<Frame, 10> frames;
    Frame frame;

    if (!frames.try_get(frame)) {
        std::cout << "get frame from ring buffer failed!" << std::endl;
    }

    for (size_t i = 0; i < 5; i++) {
        frame.index = i;
        frame.data[0] = 'a' + i;
        frame.data[1] = '\0';
        frames.put(frame);
    }
    while (frames.try_get(frame)) {
        std::cout << "Frame " << frame.index << ": " << frame.data << std::endl;
    }

    for (size_t i = 0; i < 26; i++) {
        frame.index = i;
        frame.data[0] = 'a' + i;
        frame.data[1] = '\0';
        frames.put(frame);
    }
    while (frames.try_get(frame)) {
        std::cout << "Frame " << frame.index << ": " << frame.data << std::endl;
    }
}
