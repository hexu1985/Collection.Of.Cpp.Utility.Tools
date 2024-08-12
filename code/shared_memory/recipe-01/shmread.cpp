#include <sstream>
#include <gflags/gflags.h>

#include "shared_memory_object.hpp"
#include "dump_functions.hpp"

DEFINE_string(name, "shm_test", "shared memory name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "read shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    SharedMemoryObject shdmem(FLAGS_name.c_str(), true);
    const uint8_t* ptr = (const uint8_t*) shdmem.get_address();
    size_t size = shdmem.get_size();

    dump_hex(ptr, size, "");
    printf("\n");

    return 0;
}
