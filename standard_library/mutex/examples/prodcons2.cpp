#include <thread>
#include <mutex>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#define	MAXNITEMS 		1000000
#define	MAXNTHREADS			100

int		nitems;			/* read-only by producer and consumer */
struct {
  std::mutex mutex;
  int	buff[MAXNITEMS];
  int	nput;
  int	nval;
} shared; 

void produce(int*);
void consume();

int
main(int argc, char **argv)
{
	int			i, nthreads, count[MAXNTHREADS];
    std::thread thr_produce[MAXNTHREADS], thr_consume;

	if (argc != 3) {
		printf("usage: prodcons2 <#items> <#threads>\n");
        exit(1);
    }
	nitems = std::min(atoi(argv[1]), MAXNITEMS);
	nthreads = std::min(atoi(argv[2]), MAXNTHREADS);

		/* 4start all the producer threads */
	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
        thr_produce[i] = std::thread(produce, &count[i]);
	}

		/* 4wait for all the producer threads */
	for (i = 0; i < nthreads; i++) {
        thr_produce[i].join();
		printf("count[%d] = %d\n", i, count[i]);	
	}

		/* 4start, then wait for the consumer thread */
    thr_consume = std::thread(consume);
    thr_consume.join();

	exit(0);
}
/* end main */

/* include producer */
void produce(int* arg)
{
	for ( ; ; ) {
        std::unique_lock<std::mutex> lck(shared.mutex);
		if (shared.nput >= nitems) {
			return;		/* array is full, we're done */
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
        lck.unlock();
		*arg += 1;
	}
}

void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		if (shared.buff[i] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	}
}
/* end producer */
