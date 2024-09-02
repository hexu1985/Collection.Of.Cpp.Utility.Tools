#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "/mq_test", "message queue name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "remove message queue\n";
    return os.str();
}

int main(int argc, char **argv) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

	Mq_unlink(FLAGS_name.c_str());

    return 0;
}
