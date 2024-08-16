#include <thread>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

using namespace boost::interprocess;

#define	NBUFF	 	 10
#define	MAXNTHREADS	100

int		nitems, nproducers, nconsumers;		/* read-only */

struct {	/* data shared by producers and consumers */
  int	buff[NBUFF];
  int	nput;			/* item number: 0, 1, 2, ... */
  int	nputval;		/* value to store in buff[] */
  int	nget;			/* item number: 0, 1, 2, ... */
  int	ngetval;		/* value fetched from buff[] */
  interprocess_semaphore *mutex, *nempty, *nstored;
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
	shared.mutex = new interprocess_semaphore{1};
	shared.nempty = new interprocess_semaphore{NBUFF};
	shared.nstored = new interprocess_semaphore{0};

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

    delete shared.mutex;
    delete shared.nempty;
    delete shared.nstored;
	exit(0);
}
/* end main */

/* include produce */
void produce(int* arg)
{
	for ( ; ; ) {
		shared.nempty->wait();	/* wait for at least 1 empty slot */
		shared.mutex->wait();

		if (shared.nput >= nitems) {
            shared.nstored->post();	/* let consumers terminate */
            shared.nempty->post();
            shared.mutex->post();
			return;			/* all done */
		}

		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;

		shared.mutex->post();
		shared.nstored->post();	/* 1 more stored item */
		*arg += 1;
	}
}
/* end produce */

/* include consume */
void consume(int* arg)
{
	int		i;

	for ( ; ; ) {
		shared.nstored->wait();		/* wait for at least 1 stored item */
		shared.mutex->wait();

		if (shared.nget >= nitems) {
            shared.nstored->post();
            shared.mutex->post();
			return;			/* all done */
		}

		i = shared.nget % NBUFF;
		if (shared.buff[i] != shared.ngetval)
			printf("error: buff[%d] = %d\n", i, shared.buff[i]);
		shared.nget++;
		shared.ngetval++;

		shared.mutex->post();
		shared.nempty->post();		/* 1 more empty slot */
		*arg += 1;
	}
}
/* end consume */
