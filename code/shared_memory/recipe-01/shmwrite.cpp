#include <sstream>
#include <gflags/gflags.h>

#include "shared_memory_object.hpp"

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

    SharedMemoryObject shared_memory(FLAGS_name.c_str());
    uint8_t* ptr = (uint8_t*) shared_memory.get_address();
    size_t size = shared_memory.get_size();

    for (int i = 0; i < size; i++)
        *ptr++ = i % 256;

    return 0;
}
