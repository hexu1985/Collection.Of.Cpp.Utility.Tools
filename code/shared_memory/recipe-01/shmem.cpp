#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "shared_memory.hpp"

const char* kSharedMemPath = "/sample_point";
const size_t kPayloadSize = 16;

using namespace std::literals;

template<class T>
class SharedMem {
    SharedMemory shared_memory;
    const char* name;

public:
    SharedMem(const char* name, bool owner=false): shared_memory(SharedMemory::create(name, sizeof(T))) {
        this->name = owner ? name : nullptr;
    }

    ~SharedMem() {
        shared_memory.clear();
        if (name) {
            std::cout << "Remove shared mem instance " << name << std::endl;
            SharedMemory::remove(name);
        }
    }

    T& get() const {
      return *((T* ) shared_memory.get_address());
    }
};


struct Payload {
  uint32_t index;
  uint8_t raw[kPayloadSize];
};


void producer() {
  SharedMem<Payload> writer(kSharedMemPath);
  Payload& pw = writer.get();
  for (int i = 0; i < 5; i++) {
    pw.index = i;
    std::fill_n(pw.raw, sizeof(pw.raw) - 1, 'a' + i);
    pw.raw[sizeof(pw.raw) - 1] = '\0';
    std::this_thread::sleep_for(150ms);
  }
}

void consumer() {
  SharedMem<Payload> point_reader(kSharedMemPath, true);
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
