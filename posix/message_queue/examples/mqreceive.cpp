#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "/mq_test", "message queue name");
DEFINE_bool(nonblock, false, "receive message nonblock from queue");
DEFINE_bool(dump, false, "dump message");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--nonblock] [--name NAME]\n\n"
        << "receive message from queue\n";
    return os.str();
}

int main(int argc, char **argv) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

	int flags = O_RDONLY;
    if (FLAGS_nonblock) {
        flags |= O_NONBLOCK;
    }

	mqd_t mqd = Mq_open(FLAGS_name.c_str(), flags);

	struct mq_attr attr;
	Mq_getattr(mqd, &attr);

	void* buff = malloc(attr.mq_msgsize);

    unsigned int prio;
	ssize_t n = Mq_receive(mqd, (char*) buff, attr.mq_msgsize, &prio);
	printf("read %ld bytes, priority = %u\n", (long) n, prio);
    if (FLAGS_dump && n > 0) {
        std::string content((const char*) buff, n);
        std::cout << "content[" << content << "]" << std::endl;
    }

	return 0;
}
