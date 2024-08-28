#pragma once

#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <mqueue.h>      /* Posix message queues */

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

mqd_t    Mq_open(const char *, int, ...);
void     Mq_close(mqd_t);
void     Mq_unlink(const char *);
void     Mq_send(mqd_t, const char *, size_t, unsigned int);
ssize_t  Mq_receive(mqd_t, char *, size_t, unsigned int *);
void     Mq_notify(mqd_t, const struct sigevent *);
void     Mq_getattr(mqd_t, struct mq_attr *);
void     Mq_setattr(mqd_t, const struct mq_attr *, struct mq_attr *);
