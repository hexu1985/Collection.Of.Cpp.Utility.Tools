#include <iostream>

#include "ring_buffer.hpp"

struct Frame {
    uint32_t index;
    uint8_t  data[1024];
};

int main() {
    RingBuffer<Frame, 10> frames;

    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    try {
        const Frame& frame = frames.pull();
    } catch (std::runtime_error e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    for (size_t i = 0; i < 5; i++) {
        Frame& out = frames.push();
        out.index = i;
        out.data[0] = 'a' + i;
        out.data[1] = '\0';
    }
    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    while (frames.has_data()) {
        const Frame& in = frames.pull();
        std::cout << "Frame " << in.index << ": " << in.data << std::endl;
    }

    for (size_t i = 0; i < 26; i++) {
        Frame& out = frames.push();
        out.index = i;
        out.data[0] = 'a' + i;
        out.data[1] = '\0';
    }
    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    while (frames.has_data()) {
        const Frame& in = frames.pull();
        std::cout << "Frame " << in.index << ": " << in.data << std::endl;
    }
}
