#include <sstream>
#include <gflags/gflags.h>

#include "shared_memory.hpp"

DEFINE_string(name, "shm_test", "shared memory name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "write shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (!SharedMemory::exists(FLAGS_name.c_str())) {
        printf("shared memory of %s not exists!\n", FLAGS_name.c_str());
        return -1;
    }

    SharedMemory shared_memory = SharedMemory::open_read_write(FLAGS_name.c_str());
    size_t size = shared_memory.size();
    uint8_t* ptr = (uint8_t*) shared_memory.map(size);

    for (int i = 0; i < size; i++)
        *ptr++ = i % 256;

    return 0;
}
