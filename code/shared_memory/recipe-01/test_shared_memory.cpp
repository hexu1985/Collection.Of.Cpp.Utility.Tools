#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#include "shared_memory.hpp"

const char* kSharedMemPath = "/sample_point";
const size_t kPayloadSize = 16;

using namespace std::literals;

struct Payload {
    uint32_t index;
    uint8_t raw[kPayloadSize];
};


void producer() {
    SharedMemory<Payload> writer(kSharedMemPath);
    Payload& pw = writer.get();
    for (int i = 0; i < 5; i++) {
        pw.index = i;
        std::fill_n(pw.raw, sizeof(pw.raw) - 1, 'a' + i);
        pw.raw[sizeof(pw.raw) - 1] = '\0';
        std::this_thread::sleep_for(150ms);
    }
}

void consumer() {
    SharedMemory<Payload> point_reader(kSharedMemPath, true);
    Payload& pr = point_reader.get();
    for (int i = 0; i < 10; i++) {
        std::cout << "Read data frame " << pr.index << ": " << pr.raw << std::endl;
        std::this_thread::sleep_for(100ms);
    }
}

int main() {

    if (fork()) {
        consumer();
    } else {
        producer();
    }
}

