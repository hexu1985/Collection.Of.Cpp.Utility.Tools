#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "interprocess_semaphore.hpp"

#define	NBUFF	 10

int		nitems;					/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
    int	buff[NBUFF];
    InterprocessSemaphore *mutex, *nempty, *nstored;
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
    shared.mutex = new InterprocessSemaphore{1};
    shared.nempty = new InterprocessSemaphore{NBUFF};
    shared.nstored = new InterprocessSemaphore{0};

    std::thread thr_produce(produce);
    std::thread thr_consume(consume);

    thr_produce.join();
    thr_consume.join();

    delete shared.mutex;
    delete shared.nempty;
    delete shared.nstored;
    exit(0);
}

void produce()
{
    int		i;

    for (i = 0; i < nitems; i++) {
        shared.nempty->wait();	/* wait for at least 1 empty slot */
        shared.mutex->wait();
        shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
        shared.mutex->post();
        shared.nstored->post();	/* 1 more stored item */
    }
}

void consume()
{
    int		i;

    for (i = 0; i < nitems; i++) {
        shared.nstored->wait();		/* wait for at least 1 stored item */
        shared.mutex->wait();
        if (shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        shared.mutex->post();
        shared.nempty->post();		/* 1 more empty slot */
    }
}
