#include <iostream>

#include "ring_buffer.hpp"

struct Frame {
    uint32_t index;
    uint8_t  data[1024];
};

int main() {
    RingBuffer<Frame, 10> frames;
    Frame frame;

    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    try {
        frames.get(frame);
    } catch (std::runtime_error e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    for (size_t i = 0; i < 5; i++) {
        frame.index = i;
        frame.data[0] = 'a' + i;
        frame.data[1] = '\0';
        frames.put(frame);
    }
    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    while (frames.has_data()) {
        frames.get(frame);
        std::cout << "Frame " << frame.index << ": " << frame.data << std::endl;
    }

    for (size_t i = 0; i < 26; i++) {
        frame.index = i;
        frame.data[0] = 'a' + i;
        frame.data[1] = '\0';
        frames.put(frame);
    }
    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    while (frames.has_data()) {
        frames.get(frame);
        std::cout << "Frame " << frame.index << ": " << frame.data << std::endl;
    }
}
