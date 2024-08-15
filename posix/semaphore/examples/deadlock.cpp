/* include main */
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	NBUFF	  2
#define	SEM_MUTEX	"mutex"	 	/* these are args to px_ipc_name() */
#define	SEM_NEMPTY	"nempty"
#define	SEM_NSTORED	"nstored"

int		nitems;					/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
  int	buff[NBUFF];
  sem_t	*mutex, *nempty, *nstored;
} shared;

void produce();
void consume();

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: deadlock <#items>\n");
        exit(1);
    }
	nitems = atoi(argv[1]);

		/* 4create three semaphores */
	shared.mutex = Sem_open(SEM_MUTEX, O_CREAT | O_EXCL,
							FILE_MODE, 1);
	shared.nempty = Sem_open(SEM_NEMPTY, O_CREAT | O_EXCL,
							 FILE_MODE, NBUFF);
	shared.nstored = Sem_open(SEM_NSTORED, O_CREAT | O_EXCL,
							  FILE_MODE, 0);

    std::thread thr_produce(produce);
    std::thread thr_consume(consume);

    thr_produce.join();
    thr_consume.join();

	Sem_unlink(SEM_MUTEX);
	Sem_unlink(SEM_NEMPTY);
	Sem_unlink(SEM_NSTORED);
	exit(0);
}
/* end main */

/* include prodcons */
void produce()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		printf("prod: calling sem_wait(nempty)\n");
		Sem_wait(shared.nempty);	/* wait for at least 1 empty slot */
		printf("prod: got sem_wait(nempty)\n");
		printf("prod: calling sem_wait(mutex)\n");
		Sem_wait(shared.mutex);
		printf("prod: got sem_wait(mutex), storing %d\n", i);
		shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
		Sem_post(shared.mutex);
		Sem_post(shared.nstored);	/* 1 more stored item */
	}
}

void consume()
{
	int		i;

	for (i = 0; i < nitems; i++) {
		printf("cons: calling sem_wait(mutex)\n");
		Sem_wait(shared.mutex);
		printf("cons: got sem_wait(mutex)\n");
		printf("cons: calling sem_wait(nstored)\n");
		Sem_wait(shared.nstored);		/* wait for at least 1 stored item */
		printf("cons: got sem_wait(nstored)\n");
		printf("cons: fetched %d\n", shared.buff[i % NBUFF]);
		Sem_post(shared.mutex);
		Sem_post(shared.nempty);		/* 1 more empty slot */
	}
	printf("\n");
}
/* end prodcons */
