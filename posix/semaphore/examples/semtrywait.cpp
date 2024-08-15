#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "sem_test", "semaphore name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "try wait semaphore\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    sem_t* sem = Sem_open(FLAGS_name.c_str(), 0);
    if (Sem_trywait(sem) == 0) {
        int val;
        Sem_getvalue(sem, &val);
        printf("pid %ld has semaphore, value = %d\n", (long) getpid(), val);
    } else {
        printf("pid %ld hasn't semaphore\n", (long) getpid());
    }

    Sem_close(sem);
    return 0;
}
