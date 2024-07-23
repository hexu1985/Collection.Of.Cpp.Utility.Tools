#include <sstream>
#include <gflags/gflags.h>
#include <boost/interprocess/shared_memory_object.hpp>

using namespace boost::interprocess;

DEFINE_string(name, "shm_test", "shared memory name");
DEFINE_uint32(length, 1024, "shared memory length");
DEFINE_bool(check_exists, false, "check shared memory already exists");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--check_exists] [--name NAME] [--length LENGTH]\n\n"
        << "create shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    shared_memory_object shdmem;
    if (FLAGS_check_exists) {
        shdmem = shared_memory_object{create_only, FLAGS_name.c_str(), read_write};
    } else {
        shdmem = shared_memory_object{open_or_create, FLAGS_name.c_str(), read_write};
    }
    shdmem.truncate(FLAGS_length);

    return 0;
}
