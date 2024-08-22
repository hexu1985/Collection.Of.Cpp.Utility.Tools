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
        frames.pop(frame);
    } catch (std::runtime_error e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    Frame out; 
    for (size_t i = 0; i < 5; i++) {
        out.index = i;
        out.data[0] = 'a' + i;
        out.data[1] = '\0';
        frames.push(out);
    }
    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    Frame in;
    while (frames.has_data()) {
        frames.pop(in);
        std::cout << "Frame " << in.index << ": " << in.data << std::endl;
    }

    for (size_t i = 0; i < 26; i++) {
        out.index = i;
        out.data[0] = 'a' + i;
        out.data[1] = '\0';
        frames.push(out);
    }
    std::cout << "Frames " << (frames.has_data() ? "" : "do not ")
        << "contain data" << std::endl;
    while (frames.has_data()) {
        frames.pop(in);
        std::cout << "Frame " << in.index << ": " << in.data << std::endl;
    }
}
