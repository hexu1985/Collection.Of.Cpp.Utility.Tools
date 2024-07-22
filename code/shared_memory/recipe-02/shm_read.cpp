#include <sstream>
#include <gflags/gflags.h>

#include "shared_memory_object.hpp"
#include "dump_functions.hpp"

DEFINE_string(name, "shm_test", "shared memory name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "create shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (!SharedMemoryObject::exists(FLAGS_name.c_str())) {
        printf("shared memory of %s not exists!\n", FLAGS_name.c_str());
        return -1;
    }

    SharedMemoryObject shared_memory(FLAGS_name.c_str());
    size_t size = shared_memory.size();
    const uint8_t* ptr = (const uint8_t*) shared_memory.map(size, true);

    dump_hex(ptr, size, "");
    printf("\n");

    return 0;
}
