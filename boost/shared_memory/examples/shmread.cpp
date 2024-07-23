#include <iostream>
#include <sstream>
#include <gflags/gflags.h>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "dump_functions.hpp"

using namespace boost::interprocess;

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

    shared_memory_object shdmem{open_only, FLAGS_name.c_str(), read_only};
    offset_t size;
    shdmem.get_size(size);

    mapped_region region{shdmem, read_only};
    const uint8_t* ptr = static_cast<const uint8_t*>(region.get_address());

    dump_hex(ptr, size, "");
    printf("\n");

    return 0;
}
