#include <atomic>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#include <unistd.h>

#include "shared_memory.hpp"

const char* kSharedMemPath = "/sample_point";
const size_t kPayloadSize = 16;

using namespace std::literals;

template <typename T>
using SharedMem = SharedMemory<T>;

struct Payload {
  std::atomic_bool data_ready;
  std::atomic_bool data_processed;
  uint32_t index;
  uint8_t raw[kPayloadSize];
};


void producer() {
  SharedMem<Payload> writer(kSharedMemPath);
  Payload& pw = writer.get();
  if (!pw.data_ready.is_lock_free()) {
    throw std::runtime_error("Timestamp is not lock-free");
  }
  for (int i = 0; i < 10; i++) {
    pw.data_processed.store(false);
    pw.index = i;
    std::fill_n(pw.raw, sizeof(pw.raw) - 1, 'a' + i);
    pw.raw[sizeof(pw.raw) - 1] = '\0';
    std::this_thread::sleep_for(150ms);
    pw.data_ready.store(true);
    while(!pw.data_processed.load());
  }
}

void consumer() {
  SharedMem<Payload> point_reader(kSharedMemPath);
  Payload& pr = point_reader.get();
  if (!pr.data_ready.is_lock_free()) {
    throw std::runtime_error("Timestamp is not lock-free");
  }
  for (int i = 0; i < 10; i++) {
    while(!pr.data_ready.load());
    pr.data_ready.store(false);
    std::cout << "Read data frame " << pr.index << ": " << pr.raw << std::endl;
    std::this_thread::sleep_for(100ms);
    pr.data_processed.store(true);
  }
  SharedMemoryObject::remove(kSharedMemPath);
}

int main() {

  if (fork()) {
    consumer();
  } else {
    producer();
  }
}
