#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

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

    int flags = O_RDWR | O_CREAT;
    if (FLAGS_check_exists) {
        flags |= O_EXCL;
    }

    int fd = Shm_open(FLAGS_name.c_str(), flags, FILE_MODE);
    Ftruncate(fd, FLAGS_length);

    void* ptr = Mmap(NULL, FLAGS_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    return 0;
}
