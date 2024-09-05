#pragma once

#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <mqueue.h>      /* Posix message queues */
#include <signal.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

mqd_t    Mq_open(const char *, int, ...);
void     Mq_close(mqd_t);
void     Mq_unlink(const char *);
void     Mq_send(mqd_t, const char *, size_t, unsigned int);
ssize_t  Mq_receive(mqd_t, char *, size_t, unsigned int *);
void     Mq_notify(mqd_t, const struct sigevent *);
void     Mq_getattr(mqd_t, struct mq_attr *);
void     Mq_setattr(mqd_t, const struct mq_attr *, struct mq_attr *);


typedef void    Sigfunc(int);   /* for signal handlers */

Sigfunc *signal(int, Sigfunc *);
Sigfunc *Signal(int, Sigfunc *);

long     Sysconf(int);

void     Sigaddset(sigset_t *, int);
void     Sigdelset(sigset_t *, int);
void     Sigemptyset(sigset_t *);
void     Sigfillset(sigset_t *);
int      Sigismember(const sigset_t *, int);
void     Sigpending(sigset_t *);
void     Sigprocmask(int, const sigset_t *, sigset_t *);
void     Sigwait(const sigset_t *set, int *signo);

void     Pipe(int *fds);
ssize_t  Read(int, void *, size_t);
void     Write(int, void *, size_t);
int      Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
