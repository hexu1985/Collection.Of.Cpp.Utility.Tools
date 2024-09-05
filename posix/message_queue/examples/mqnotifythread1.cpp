#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <memory>

#include "posix_api.hpp"

mqd_t	mqd;
struct mq_attr	attr;
struct sigevent	sigev;

static void	notify_thread(union sigval);		/* our thread function */

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: mqnotifythread1 <name>\n");
        exit(1);
    }

	mqd = Mq_open(argv[1], O_RDONLY | O_NONBLOCK);
	Mq_getattr(mqd, &attr);

	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_value.sival_ptr = NULL;
	sigev.sigev_notify_function = notify_thread;
	sigev.sigev_notify_attributes = NULL;
	Mq_notify(mqd, &sigev);

	for ( ; ; )
		pause();		/* each new thread does everything */

	exit(0);
}

static void notify_thread(union sigval arg) {
	ssize_t	n;

	printf("notify_thread started\n");
    std::unique_ptr<char[]> buff{new char[attr.mq_msgsize]};
	Mq_notify(mqd, &sigev);			/* reregister */

	while ( (n = mq_receive(mqd, buff.get(), attr.mq_msgsize, NULL)) >= 0) {
		printf("read %ld bytes\n", (long) n);
	}
	if (errno != EAGAIN) {
        throw std::system_error(errno, std::system_category(), "mq_receive error");
    }

	pthread_exit(NULL);
}
