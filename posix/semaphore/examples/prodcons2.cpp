#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	NBUFF	 10

int		nitems;					/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
  int	buff[NBUFF];
  sem_t	mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

void produce();
void consume();

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: prodcons2 <#items>\n");
        exit(1);
    }
	nitems = atoi(argv[1]);

		/* 4initialize three semaphores */
	Sem_init(&shared.mutex, 0, 1);
	Sem_init(&shared.nempty, 0, NBUFF);
	Sem_init(&shared.nstored, 0, 0);

    std::thread thr_produce(produce);
    std::thread thr_consume(consume);

    thr_produce.join();
    thr_consume.join();

	Sem_destroy(&shared.mutex);
	Sem_destroy(&shared.nempty);
	Sem_destroy(&shared.nstored);
	exit(0);
}

void produce()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		Sem_wait(&shared.nempty);	/* wait for at least 1 empty slot */
		Sem_wait(&shared.mutex);
		shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
		Sem_post(&shared.mutex);
		Sem_post(&shared.nstored);	/* 1 more stored item */
	}
}

void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		Sem_wait(&shared.nstored);		/* wait for at least 1 stored item */
		Sem_wait(&shared.mutex);
		if (shared.buff[i % NBUFF] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
		Sem_post(&shared.mutex);
		Sem_post(&shared.nempty);		/* 1 more empty slot */
	}
}
