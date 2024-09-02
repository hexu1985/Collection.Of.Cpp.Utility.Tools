#include <cstdio>
#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "/mq_test", "message queue name");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME]\n\n"
        << "get attribute of message queue\n";
    return os.str();
}

int main(int argc, char **argv) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

	mqd_t mqd = Mq_open(FLAGS_name.c_str(), O_RDONLY);

	struct mq_attr	attr;
	Mq_getattr(mqd, &attr);
	printf("max #msgs = %ld, max #bytes/msg = %ld, "
		   "#currently on queue = %ld\n",
		   attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

	Mq_close(mqd);
    return 0;
}
