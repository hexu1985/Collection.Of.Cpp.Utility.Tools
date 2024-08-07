#include "posix_api.hpp"
#include <errno.h>
#include <stdarg.h>
#include <system_error>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

sem_t* Sem_open(const char* name, int oflag, ...) {
	sem_t*  sem;
	va_list ap;
	mode_t  mode;
	unsigned int    value;

	if (oflag & O_CREAT) {
		va_start(ap, oflag);		/* init ap to final named argument */
		mode = va_arg(ap, mode_t);
		value = va_arg(ap, unsigned int);
		if ((sem = sem_open(name, oflag, mode, value)) == SEM_FAILED) {
            throw std::system_error(errno, std::system_category(), 
                    format("sem_open error for {}", name));
        }
		va_end(ap);
	} else {
		if ((sem = sem_open(name, oflag)) == SEM_FAILED) {
            throw std::system_error(errno, std::system_category(), 
                    format("sem_open error for {}", name));
        }
	}
	return(sem);
}

void Sem_close(sem_t* sem) {
	if (sem_close(sem) == -1) {
        throw std::system_error(errno, std::system_category(), "sem_close error");
    }
}

void Sem_unlink(const char* name) {
	if (sem_unlink(name) == -1) {
        throw std::system_error(errno, std::system_category(), "sem_unlink error");
    }
}

void Sem_wait(sem_t* sem) {
	if (sem_wait(sem) == -1) {
        throw std::system_error(errno, std::system_category(), "sem_wait error");
    }
}

int Sem_trywait(sem_t* sem) {
	int rc;

	if ((rc = sem_trywait(sem)) == -1 && errno != EAGAIN) {
        throw std::system_error(errno, std::system_category(), "sem_trywait error");
    }
	return(rc);
}

void Sem_post(sem_t* sem) {
	if (sem_post(sem) == -1) {
        throw std::system_error(errno, std::system_category(), "sem_post error");
    }
}

void Sem_getvalue(sem_t* sem, int* valp) {
	if (sem_getvalue(sem, valp) == -1) {
        throw std::system_error(errno, std::system_category(), "sem_getvalue error");
    }
}

