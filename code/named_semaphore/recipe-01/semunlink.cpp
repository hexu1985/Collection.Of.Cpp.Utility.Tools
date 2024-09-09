#include <sstream>
#include <gflags/gflags.h>
#include "named_semaphore.hpp"

DEFINE_string(name, "sem_test", "semaphore name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "remove semaphore\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    NamedSemaphore::remove(FLAGS_name.c_str());

    return 0;
}
