#include <sstream>
#include <gflags/gflags.h>
#include "named_semaphore.hpp"

DEFINE_string(name, "sem_test", "semaphore name");
DEFINE_uint32(initial_value, 1, "initial value");
DEFINE_bool(check_exists, false, "check semaphore already exists");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--check_exists] [--name NAME] [--initial_value INITIAL_VALUE]\n\n"
        << "create semaphore\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (FLAGS_check_exists) {
        NamedSemaphore sem{NamedSemaphore::create_only(FLAGS_name.c_str(), FLAGS_initial_value)};
    } else {
        NamedSemaphore sem{NamedSemaphore::open_or_create(FLAGS_name.c_str(), FLAGS_initial_value)};
    }

    return 0;
}
