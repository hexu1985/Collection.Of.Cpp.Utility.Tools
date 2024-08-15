#include <thread>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	BUFFSIZE		8192
#define	NBUFF	 8

struct {	/* data shared by producer and consumer */
  struct {
    char	data[BUFFSIZE];			/* a buffer */
    ssize_t	n;						/* count of #bytes in the buffer */
  } buff[NBUFF];					/* NBUFF of these buffers/counts */
  sem_t	mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

int		fd;							/* input file to copy to stdout */
void produce();
void consume();

int
main(int argc, char **argv)
{
    std::thread thr_produce, thr_consume;

	if (argc != 2) {
		printf("usage: mycat2 <pathname>\n");
        exit(1);
    }

	fd = Open(argv[1], O_RDONLY);

		/* 4initialize three semaphores */
	Sem_init(&shared.mutex, 0, 1);
	Sem_init(&shared.nempty, 0, NBUFF);
	Sem_init(&shared.nstored, 0, 0);

		/* 4one producer thread, one consumer thread */
    thr_produce = std::thread(produce);
    thr_consume = std::thread(consume);

    thr_produce.join();
    thr_consume.join();

	Sem_destroy(&shared.mutex);
	Sem_destroy(&shared.nempty);
	Sem_destroy(&shared.nstored);
	exit(0);
}
/* end main */

/* include prodcons */
void produce()
{
	int		i;

	for (i = 0; ; ) {
		Sem_wait(&shared.nempty);	/* wait for at least 1 empty slot */

		Sem_wait(&shared.mutex);
			/* 4critical region */
		Sem_post(&shared.mutex);

		shared.buff[i].n = Read(fd, shared.buff[i].data, BUFFSIZE);
		if (shared.buff[i].n == 0) {
			Sem_post(&shared.nstored);	/* 1 more stored item */
			return;
		}
		if (++i >= NBUFF)
			i = 0;					/* circular buffer */

		Sem_post(&shared.nstored);	/* 1 more stored item */
	}
}

void consume()
{
	int		i;

	for (i = 0; ; ) {
		Sem_wait(&shared.nstored);		/* wait for at least 1 stored item */

		Sem_wait(&shared.mutex);
			/* 4critical region */
		Sem_post(&shared.mutex);

		if (shared.buff[i].n == 0)
			return;
		Write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n);
		if (++i >= NBUFF)
			i = 0;					/* circular buffer */

		Sem_post(&shared.nempty);		/* 1 more empty slot */
	}
}
/* end prodcons */
