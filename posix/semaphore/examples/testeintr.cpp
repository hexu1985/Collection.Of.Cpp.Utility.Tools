#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	NAME	"testeintr"

static void	sig_alrm(int);

int main(int argc, char **argv) {
	sem_t	*sem1, sem2;

		/* 4first test a named semaphore */
	sem_unlink(NAME);
	sem1 = Sem_open(NAME, O_RDWR | O_CREAT | O_EXCL,
					FILE_MODE, 0);

	Signal(SIGALRM, sig_alrm);
	alarm(2);
	if (sem_wait(sem1) == 0) {
		printf("sem_wait returned 0?\n");
    } else {
        printf("sem1 sem_wait error\n");
    }
	Sem_close(sem1);

		/* 4now a memory-based semaphore with process scope */
	Sem_init(&sem2, 1, 0);
	alarm(2);
	if (sem_wait(&sem2) == 0) {
		printf("sem_wait returned 0?\n");
    } else {
        printf("sem2 sem_wait error\n");
    }
	Sem_destroy(&sem2);

	exit(0);
}

static void sig_alrm(int signo) {
	printf("SIGALRM caught\n");
	return;
}
