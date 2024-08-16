#include <thread>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	NBUFF	 	 10
#define	MAXNTHREADS	100

int		nitems, nproducers;		/* read-only by producer and consumer */

struct {	/* data shared by producers and consumer */
  int	buff[NBUFF];
  int	nput;
  int	nputval;
  sem_t	mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

void produce(int* arg);
void consume();

int
main(int argc, char **argv)
{
	int		i, count[MAXNTHREADS];
    std::thread thr_produce[MAXNTHREADS], thr_consume;

	if (argc != 3) {
		printf("usage: prodcons3 <#items> <#producers>\n");
        exit(1);
    }
	nitems = atoi(argv[1]);
	nproducers = std::min(atoi(argv[2]), MAXNTHREADS);

		/* 4initialize three semaphores */
	Sem_init(&shared.mutex, 0, 1);
	Sem_init(&shared.nempty, 0, NBUFF);
	Sem_init(&shared.nstored, 0, 0);

		/* 4create all producers and one consumer */
	for (i = 0; i < nproducers; i++) {
		count[i] = 0;
        thr_produce[i] = std::thread(produce, &count[i]);
	}
    thr_consume = std::thread(consume);

		/* 4wait for all producers and the consumer */
	for (i = 0; i < nproducers; i++) {
        thr_produce[i].join();
		printf("count[%d] = %d\n", i, count[i]);	
	}
    thr_consume.join();

	Sem_destroy(&shared.mutex);
	Sem_destroy(&shared.nempty);
	Sem_destroy(&shared.nstored);
	exit(0);
}
/* end main */

/* include produce */
void produce(int* arg)
{
	for ( ; ; ) {
		Sem_wait(&shared.nempty);	/* wait for at least 1 empty slot */
		Sem_wait(&shared.mutex);

		if (shared.nput >= nitems) {
			Sem_post(&shared.nempty);
			Sem_post(&shared.mutex);
			return;			/* all done */
		}

		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;

		Sem_post(&shared.mutex);
		Sem_post(&shared.nstored);	/* 1 more stored item */
		*arg += 1;
	}
}
/* end produce */

/* include consume */
void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		Sem_wait(&shared.nstored);		/* wait for at least 1 stored item */
		Sem_wait(&shared.mutex);

		if (shared.buff[i % NBUFF] != i)
			printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);

		Sem_post(&shared.mutex);
		Sem_post(&shared.nempty);		/* 1 more empty slot */
	}
}
/* end consume */
