#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>

#include <unistd.h>

#include "shared_memory.hpp"

const char* kSharedMemPath = "/sample_point";

template <typename T>
using SharedMem = SharedMemory<T>;

struct Payload {
  std::atomic_bool data_ready;
  std::atomic_bool data_processed;
  int index;
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
    std::cout << "Processing data chunk " << pr.index << std::endl;
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
