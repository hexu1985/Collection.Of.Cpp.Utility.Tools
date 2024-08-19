#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#define	MAXNITEMS 		1000000
#define	MAXNTHREADS			100

		/* globals shared by threads */
int		nitems;				/* read-only by producer and consumer */
int		buff[MAXNITEMS];
struct {
  std::mutex mutex;
  std::condition_variable cond;
  int				nput;
  int				nval;
  int				nready;
} nready; 

void produce(int*);
void consume();

/* include main */
int
main(int argc, char **argv)
{
	int			i, nthreads, count[MAXNTHREADS];
    std::thread thr_produce[MAXNTHREADS], thr_consume;

	if (argc != 3) {
		printf("usage: prodcons5 <#items> <#threads>\n");
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
        std::unique_lock<std::mutex> lck(nready.mutex);
		if (nready.nput >= nitems) {
			return;		/* array is full, we're done */
		}
		buff[nready.nput] = nready.nval;
		nready.nput++;
		nready.nval++;
		nready.nready++;
        nready.cond.notify_one();
        lck.unlock();
		*arg += 1;
	}
}

/* include consume */
void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
        std::unique_lock<std::mutex> lck(nready.mutex);
		while (nready.nready == 0) {
            nready.cond.wait(lck);
        }
		nready.nready--;
        lck.unlock();

		if (buff[i] != i)
			printf("buff[%d] = %d\n", i, buff[i]);
	}
	return;
}
/* end consume */
