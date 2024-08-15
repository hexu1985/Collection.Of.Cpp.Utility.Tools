#include <thread>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	NBUFF	 	 10
#define	MAXNTHREADS	100

int		nitems, nproducers, nconsumers;		/* read-only */

struct {	/* data shared by producers and consumers */
  int	buff[NBUFF];
  int	nput;			/* item number: 0, 1, 2, ... */
  int	nputval;		/* value to store in buff[] */
  int	nget;			/* item number: 0, 1, 2, ... */
  int	ngetval;		/* value fetched from buff[] */
  sem_t	mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

void produce(int* arg);
void consume(int* arg);
/* end globals */

/* include main */
int
main(int argc, char **argv)
{
	int		i, prodcount[MAXNTHREADS], conscount[MAXNTHREADS];
    std::thread thr_produce[MAXNTHREADS], thr_consume[MAXNTHREADS];

	if (argc != 4) {
		printf("usage: prodcons4 <#items> <#producers> <#consumers>\n");
        exit(1);
    }
	nitems = atoi(argv[1]);
	nproducers = std::min(atoi(argv[2]), MAXNTHREADS);
	nconsumers = std::min(atoi(argv[3]), MAXNTHREADS);

		/* 4initialize three semaphores */
	Sem_init(&shared.mutex, 0, 1);
	Sem_init(&shared.nempty, 0, NBUFF);
	Sem_init(&shared.nstored, 0, 0);

		/* 4create all producers and all consumers */
	for (i = 0; i < nproducers; i++) {
		prodcount[i] = 0;
        thr_produce[i] = std::thread(produce, &prodcount[i]);
	}
	for (i = 0; i < nconsumers; i++) {
		conscount[i] = 0;
        thr_consume[i] = std::thread(consume, &conscount[i]);
	}

		/* 4wait for all producers and all consumers */
	for (i = 0; i < nproducers; i++) {
        thr_produce[i].join();
		printf("producer count[%d] = %d\n", i, prodcount[i]);	
	}
	for (i = 0; i < nconsumers; i++) {
        thr_consume[i].join();
		printf("consumer count[%d] = %d\n", i, conscount[i]);	
	}

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
			Sem_post(&shared.nstored);	/* let consumers terminate */
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
void consume(int* arg)
{
	int		i;

	for ( ; ; ) {
		Sem_wait(&shared.nstored);	/* wait for at least 1 stored item */
		Sem_wait(&shared.mutex);

		if (shared.nget >= nitems) {
			Sem_post(&shared.nstored);
			Sem_post(&shared.mutex);
			return;			/* all done */
		}

		i = shared.nget % NBUFF;
		if (shared.buff[i] != shared.ngetval)
			printf("error: buff[%d] = %d\n", i, shared.buff[i]);
		shared.nget++;
		shared.ngetval++;

		Sem_post(&shared.mutex);
		Sem_post(&shared.nempty);	/* 1 more empty slot */
		*arg += 1;
	}
}
/* end consume */
