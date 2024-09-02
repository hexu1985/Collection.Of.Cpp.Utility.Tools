#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <gflags/gflags.h>

#include "posix_api.hpp"

DEFINE_string(name, "/mq_test", "message queue name");
DEFINE_uint32(message_size, 10, "message size (in bytes)");
DEFINE_string(fill_with, "a", "filling message with char");
DEFINE_uint32(priority, 0, "message priority");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--name NAME] [--message_size MESSAGE_SIZE] [--fill_with CHAR]\n\n"
        << "send message to queue\n";
    return os.str();
}

int main(int argc, char **argv) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    size_t len = FLAGS_message_size;
    unsigned int prio = FLAGS_priority;
    mqd_t mqd = Mq_open(FLAGS_name.c_str(), O_WRONLY);
    void* ptr = calloc(len, sizeof(char));
    memset(ptr, FLAGS_fill_with[0], len);
    Mq_send(mqd, (const char*) ptr, len, prio); 
    free(ptr);

    return 0;
}
