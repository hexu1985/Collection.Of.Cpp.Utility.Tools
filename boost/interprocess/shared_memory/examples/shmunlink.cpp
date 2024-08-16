#include <sstream>
#include <gflags/gflags.h>
#include <boost/interprocess/shared_memory_object.hpp>

using namespace boost::interprocess;

DEFINE_string(name, "shm_test", "shared memory name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "remove shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    shared_memory_object::remove(FLAGS_name.c_str());

    return 0;
}
