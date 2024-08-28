#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "/mq_test", "message queue name");
DEFINE_bool(check_exists, false, "check message queue already exists");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--check_exists] [--name NAME]\n\n"
        << "create message queue\n";
    return os.str();
}

int main(int argc, char **argv) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    int flags = O_RDWR | O_CREAT;
    if (FLAGS_check_exists) {
        flags |= O_EXCL;
    }

    mqd_t mqd = Mq_open(FLAGS_name.c_str(), flags, FILE_MODE, NULL);

    Mq_close(mqd);
    return 0;
}
