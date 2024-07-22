#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"
#include "dump_functions.hpp"

DEFINE_string(name, "shm_test", "shared memory name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "create shared memory\n";
    return os.str();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    int fd = Shm_open(FLAGS_name.c_str(), O_RDONLY, FILE_MODE);

    struct stat stat;
    Fstat(fd, &stat);

    const uint8_t* ptr = (const uint8_t*) Mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    Close(fd);

    dump_hex(ptr, stat.st_size, "");
    printf("\n");

    return 0;
}
