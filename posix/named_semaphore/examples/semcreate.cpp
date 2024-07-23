#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "sem_test", "semaphore name");
DEFINE_uint32(inital_value, 1, "inital value");
DEFINE_bool(check_exists, false, "check semaphore already exists");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--check_exists] [--name NAME] [--inital_value INITAL_VALUE]\n\n"
        << "create semaphore\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    int flags = O_RDWR | O_CREAT;
    if (FLAGS_check_exists) {
        flags |= O_EXCL;
    }

    sem_t* sem = Sem_open(FLAGS_name.c_str(), flags, FILE_MODE, FLAGS_inital_value);

    Sem_close(sem);
    return 0;
}
