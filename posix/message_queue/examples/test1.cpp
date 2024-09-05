#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "/mq_test", "message queue name");
DEFINE_uint32(queue_max_size, 0, "max number of messages allowed on queue");
DEFINE_uint32(message_max_size, 0, "max size of a message (in bytes)");
DEFINE_bool(check_exists, false, "check message queue already exists");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--check_exists] [--name NAME] [--queue_max_size QUEUE_MAX_SIZE] [--message_max_size MESSAGE_MAX_SIZE]\n\n"
        << "create message queue\n";
    return os.str();
}

struct mq_attr	attr;	/* mq_maxmsg and mq_msgsize both init to 0 */

int main(int argc, char **argv) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    int flags = O_RDWR | O_CREAT;
    if (FLAGS_check_exists) {
        flags |= O_EXCL;
    }

    attr.mq_maxmsg = FLAGS_queue_max_size;
    attr.mq_msgsize = FLAGS_message_max_size;

	if ((attr.mq_maxmsg != 0 && attr.mq_msgsize == 0) ||
		(attr.mq_maxmsg == 0 && attr.mq_msgsize != 0)) {
        printf("must specify both -m maxmsg and -z msgsize\n");
        exit(1);
    }

	printf("about to sleep for 30 seconds before mq_open\n");
	sleep(30);

	mqd_t mqd = Mq_open(FLAGS_name.c_str(), flags, FILE_MODE,
				  (attr.mq_maxmsg != 0) ? &attr : NULL);

	printf("mq_open OK, about to sleep for 30 more seconds\n");
	sleep(30);

	Mq_close(mqd);
	exit(0);
}
