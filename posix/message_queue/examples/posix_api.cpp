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


