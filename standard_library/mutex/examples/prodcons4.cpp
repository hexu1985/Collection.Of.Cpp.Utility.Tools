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

/* include main */
int
main(int argc, char **argv)
{
	int			i, nthreads, count[MAXNTHREADS];
    std::thread thr_produce[MAXNTHREADS], thr_consume;

	if (argc != 3) {
		printf("usage: prodcons4 <#items> <#threads>\n");
        exit(1);
    }
	nitems = std::min(atoi(argv[1]), MAXNITEMS);
	nthreads = std::min(atoi(argv[2]), MAXNTHREADS);

		/* 4create all producers and one consumer */
	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
        thr_produce[i] = std::thread(produce, &count[i]);
	}
    thr_consume = std::thread(consume);

		/* wait for all producers and the consumer */
	for (i = 0; i < nthreads; i++) {
        thr_produce[i].join();
		printf("count[%d] = %d\n", i, count[i]);	
	}
    thr_consume.join();

	exit(0);
}
/* end main */

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

/* include consume */
void consume_wait(int i)
{
	for ( ; ; ) {
        std::unique_lock<std::mutex> lck(shared.mutex);
		if (i < shared.nput) {
			return;			/* an item is ready */
		}
        std::this_thread::yield();
	}
}

void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		consume_wait(i);
		if (shared.buff[i] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	}
	return;
}
/* end consume */
