#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <memory>

#include "posix_api.hpp"

mqd_t	mqd;
std::unique_ptr<char[]> buff;
struct mq_attr	attr;
struct sigevent	sigev;

static void	sig_usr1(int);

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: mqnotifysig1 <name>\n");
        exit(1);
    }

		/* 4open queue, get attributes, allocate read buffer */
	mqd = Mq_open(argv[1], O_RDONLY);
	Mq_getattr(mqd, &attr);
    buff.reset(new char[attr.mq_msgsize]);

		/* 4establish signal handler, enable notification */
	Signal(SIGUSR1, sig_usr1);
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	Mq_notify(mqd, &sigev);

	for ( ; ; )
		pause();		/* signal handler does everything */
	exit(0);
}

static void sig_usr1(int signo) {
	ssize_t	n;

	Mq_notify(mqd, &sigev);			/* reregister first */
	n = Mq_receive(mqd, buff.get(), attr.mq_msgsize, NULL);
	printf("SIGUSR1 received, read %ld bytes\n", (long) n);
	return;
}
