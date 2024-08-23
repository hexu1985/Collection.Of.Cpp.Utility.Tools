#include <thread>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore>

using std::counting_semaphore;

#define	BUFFSIZE		8192
#define	NBUFF	 8

struct {	/* data shared by producer and consumer */
  struct {
    char	data[BUFFSIZE];			/* a buffer */
    ssize_t	n;						/* count of #bytes in the buffer */
  } buff[NBUFF];					/* NBUFF of these buffers/counts */
  counting_semaphore<> *mutex, *nempty, *nstored;
} shared;

FILE* fp;							/* input file to copy to stdout */
void produce();
void consume();

int
main(int argc, char **argv)
{
    std::thread thr_produce, thr_consume;

	if (argc != 2) {
		printf("usage: mycat2 <pathname>\n");
        exit(1);
    }

	fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("open error for %s\n", argv[1]);
        exit(2);
    }

    setbuf(fp, NULL);

		/* 4initialize three semaphores */
	shared.mutex = new counting_semaphore<>{1};
	shared.nempty = new counting_semaphore<>{NBUFF};
	shared.nstored = new counting_semaphore<>{0};

		/* 4one producer thread, one consumer thread */
    thr_produce = std::thread(produce);
    thr_consume = std::thread(consume);

    thr_produce.join();
    thr_consume.join();

    delete shared.mutex;
    delete shared.nempty;
    delete shared.nstored;
	exit(0);
}
/* end main */

/* include prodcons */
void produce()
{
	int		i;

	for (i = 0; ; ) {
		shared.nempty->acquire();	/* acquire for at least 1 empty slot */

		shared.mutex->acquire();
			/* 4critical region */
		shared.mutex->release();

		shared.buff[i].n = fread(shared.buff[i].data, 1, BUFFSIZE, fp);
		if (shared.buff[i].n == 0) {
		    shared.nstored->release();	/* 1 more stored item */
			return;
		}
		if (++i >= NBUFF)
			i = 0;					/* circular buffer */

		shared.nstored->release();	/* 1 more stored item */
	}
}

void consume()
{
	int		i;

	for (i = 0; ; ) {
		shared.nstored->acquire();		/* acquire for at least 1 stored item */

		shared.mutex->acquire();
			/* 4critical region */
		shared.mutex->release();

		if (shared.buff[i].n == 0)
			return;
		fwrite(shared.buff[i].data, 1, shared.buff[i].n, stdout);
		if (++i >= NBUFF)
			i = 0;					/* circular buffer */

		shared.nempty->release();		/* 1 more empty slot */
	}
}
/* end prodcons */
