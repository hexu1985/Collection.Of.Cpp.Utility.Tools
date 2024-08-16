#include <sstream>
#include <gflags/gflags.h>
#include <boost/interprocess/sync/named_semaphore.hpp>

using namespace boost::interprocess;

DEFINE_string(name, "sem_test", "semaphore name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "wait semaphore\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    named_semaphore sem{open_only, FLAGS_name.c_str()};
    sem.wait();
    printf("pid %ld has semaphore\n", (long) getpid());

    return 0;
}
