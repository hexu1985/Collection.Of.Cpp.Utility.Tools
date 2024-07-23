#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "shm_test", "shared memory name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "write shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    int fd = Shm_open(FLAGS_name.c_str(), O_RDWR, 0);

    struct stat stat;
    Fstat(fd, &stat);

    uint8_t* ptr = (uint8_t*) Mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);

    for (int i = 0; i < stat.st_size; i++)
        *ptr++ = i % 256;

    return 0;
}
