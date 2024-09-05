#include "posix_api.hpp"
#include <errno.h>
#include <stdarg.h>
#include <system_error>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

mqd_t Mq_open(const char *name, int oflag, ...) {
    mqd_t   mqdes;
    va_list ap;
    mode_t  mode;
    struct mq_attr  *attr;

    if (oflag & O_CREAT) {
        va_start(ap, oflag);        /* init ap to final named argument */
        mode = va_arg(ap, mode_t);
        attr = va_arg(ap, struct mq_attr *);
        if ( (mqdes = mq_open(name, oflag, mode, attr)) == (mqd_t) -1) {
            throw std::system_error(errno, std::system_category(), 
                    format("mq_open error for {}", name));
        }
        va_end(ap);
    } else {
        if ( (mqdes = mq_open(name, oflag)) == (mqd_t) -1) {
            throw std::system_error(errno, std::system_category(), 
                    format("mq_open error for {}", name));
        }
    }
    return(mqdes);
}

void Mq_close(mqd_t mqdes) {
    if (mq_close(mqdes) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_close error");
    }
}

void Mq_unlink(const char *name) {
    if (mq_unlink(name) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_unlink error");
    }
}

void Mq_getattr(mqd_t mqdes, struct mq_attr *attr) {
    if (mq_getattr(mqdes, attr) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_getattr error");
    }
}

void Mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr) {
    if (mq_setattr(mqdes, newattr, oldattr) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_setattr error");
    }
}

void Mq_send(mqd_t mqd, const char *ptr, size_t len, unsigned int prio) {
    if (mq_send(mqd, ptr, len, prio) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_send error");
    }
}

ssize_t Mq_receive(mqd_t mqd, char *ptr, size_t len, unsigned int *prio) {
    ssize_t n;

    if ( (n = mq_receive(mqd, ptr, len, prio)) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_receive error");
    }
    return(n);
}

void Mq_notify(mqd_t mqd, const struct sigevent *notification) {
    if (mq_notify(mqd, notification) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_notify error");
    }
}

Sigfunc *signal(int signo, Sigfunc *func) {
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;   /* SunOS 4.x */
#endif
    } else {
#ifdef  SA_RESTART
        act.sa_flags |= SA_RESTART;     /* SVR4, 44BSD */
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}
/* end signal */

Sigfunc *Signal(int signo, Sigfunc *func) {    /* for our signal() function */
    Sigfunc *sigfunc;

    if ( (sigfunc = signal(signo, func)) == SIG_ERR) {
        throw std::system_error(errno, std::system_category(), "signal error");
    }
    return(sigfunc);
}

long Sysconf(int name) {
    long    val;

    errno = 0;      /* in case sysconf() does not change this */
    if ( (val = sysconf(name)) == -1) {
        if (errno != 0) {
            throw std::system_error(errno, std::system_category(), "sysconf error");
        } else {
            throw std::system_error(errno, std::system_category(), 
                    format("sysconf: {} not defined", name));
        }
    }
    return(val);
}

void Sigaddset(sigset_t *set, int signo) {
    if (sigaddset(set, signo) == -1) {
        throw std::system_error(errno, std::system_category(), "sigaddset error");
    }
}

void Sigdelset(sigset_t *set, int signo) {
    if (sigdelset(set, signo) == -1) {
        throw std::system_error(errno, std::system_category(), "sigdelset error");
    }
}

void Sigemptyset(sigset_t *set) {
    if (sigemptyset(set) == -1) {
        throw std::system_error(errno, std::system_category(), "sigemptyset error");
    }
}

void Sigfillset(sigset_t *set) {
    if (sigfillset(set) == -1) {
        throw std::system_error(errno, std::system_category(), "sigfillset error");
    }
}

int Sigismember(const sigset_t *set, int signo) {
    int     n;

    if ( (n = sigismember(set, signo)) == -1) {
        throw std::system_error(errno, std::system_category(), "sigismember error");
    }
    return(n);
}

void Sigpending(sigset_t *set) {
    if (sigpending(set) == -1) {
        throw std::system_error(errno, std::system_category(), "sigpending error");
    }
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oset) {
    if (sigprocmask(how, set, oset) == -1) {
        throw std::system_error(errno, std::system_category(), "sigprocmask error");
    }
}

void Sigwait(const sigset_t *set, int *signo) {
    int     n;

    if ( (n = sigwait(set, signo)) == 0)
        return;
    errno = n;
    throw std::system_error(errno, std::system_category(), "sigwait error");
}

void Pipe(int *fds) {
    if (pipe(fds) < 0) {
        throw std::system_error(errno, std::system_category(), "pipe error");
    }
}

ssize_t Read(int fd, void *ptr, size_t nbytes) {
    ssize_t     n;

    if ( (n = read(fd, ptr, nbytes)) == -1) {
        throw std::system_error(errno, std::system_category(), "read error");
    }
    return(n);
}

void Write(int fd, void *ptr, size_t nbytes) {
    if (write(fd, ptr, nbytes) != nbytes) {
        throw std::system_error(errno, std::system_category(), "write error");
    }
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout) {
    int     n;

again:
    if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0) {
        if (errno == EINTR)
            goto again;
        else {
            throw std::system_error(errno, std::system_category(), "select error");
        }
    } else if (n == 0 && timeout == NULL) {
        throw std::runtime_error("select returned 0 with no timeout");
    }
    return(n);      /* can return 0 on timeout */
}

