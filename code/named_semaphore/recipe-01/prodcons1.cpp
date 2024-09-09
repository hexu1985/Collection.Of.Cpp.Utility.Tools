#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "named_semaphore.hpp"

#define	NBUFF	 10
#define	SEM_MUTEX	"mutex"	 	/* these are args to px_ipc_name() */
#define	SEM_NEMPTY	"nempty"
#define	SEM_NSTORED	"nstored"

int		nitems;					/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
  int	buff[NBUFF];
  NamedSemaphore mutex, nempty, nstored;
} shared;

void produce();
void consume();

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: prodcons1 <#items>\n");
        exit(1);
    }
	nitems = atoi(argv[1]);

		/* 4create three semaphores */
	shared.mutex = NamedSemaphore::create_only(SEM_MUTEX, 1);
	shared.nempty = NamedSemaphore::create_only(SEM_NEMPTY, NBUFF);
	shared.nstored = NamedSemaphore::create_only(SEM_NSTORED, 0);

		/* 4create one producer thread and one consumer thread */
    std::thread thr_produce(produce);
    std::thread thr_consume(consume);

		/* 4wait for the two threads */
    thr_produce.join();
    thr_consume.join();

		/* 4remove the semaphores */
    NamedSemaphore::remove(SEM_MUTEX);
    NamedSemaphore::remove(SEM_NEMPTY);
    NamedSemaphore::remove(SEM_NSTORED);

	exit(0);
}
/* end main */

/* include prodcons */
void produce()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		shared.nempty.wait();	/* wait for at least 1 empty slot */
		shared.mutex.wait();
		shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
		shared.mutex.post();
		shared.nstored.post();	/* 1 more stored item */
	}
}

void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		shared.nstored.wait();		/* wait for at least 1 stored item */
		shared.mutex.wait();
		if (shared.buff[i % NBUFF] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
		shared.mutex.post();
		shared.nempty.post();		/* 1 more empty slot */
	}
}
/* end prodcons */
