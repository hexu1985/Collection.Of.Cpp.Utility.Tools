#include <thread>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

using namespace boost::interprocess;

#define	NBUFF	 	 10
#define	MAXNTHREADS	100

int		nitems, nproducers;		/* read-only by producer and consumer */

struct {	/* data shared by producers and consumer */
    int	buff[NBUFF];
    int	nput;
    int	nputval;
    interprocess_semaphore *mutex, *nempty, *nstored;
} shared;

void produce(int* arg);
void consume();

int main(int argc, char **argv)
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
    shared.mutex = new interprocess_semaphore{1};
    shared.nempty = new interprocess_semaphore{NBUFF};
    shared.nstored = new interprocess_semaphore{0};

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
void consume()
{
    int		i;

    for (i = 0; i < nitems; i++) {
        shared.nstored->wait();		/* wait for at least 1 stored item */
        shared.mutex->wait();

        if (shared.buff[i % NBUFF] != i)
            printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);

        shared.mutex->post();
        shared.nempty->post();		/* 1 more empty slot */
    }
}
/* end consume */
