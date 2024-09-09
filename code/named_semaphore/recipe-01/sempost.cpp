#include <sstream>
#include <gflags/gflags.h>
#include "named_semaphore.hpp"

DEFINE_string(name, "sem_test", "semaphore name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "post semaphore\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    NamedSemaphore sem{NamedSemaphore::open_only(FLAGS_name.c_str())};
    sem.post();

    return 0;
}
