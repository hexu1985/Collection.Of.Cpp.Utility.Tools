#include "posix_api.hpp"
#include <errno.h>
#include <stdarg.h>
#include <system_error>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

mqd_t Mq_open(const char *name, int oflag, ...) {
    mqd_t   mqd;
    va_list ap;
    mode_t  mode;
    struct mq_attr  *attr;

    if (oflag & O_CREAT) {
        va_start(ap, oflag);        /* init ap to final named argument */
        mode = va_arg(ap, mode_t);
        attr = va_arg(ap, struct mq_attr *);
        if ( (mqd = mq_open(name, oflag, mode, attr)) == (mqd_t) -1) {
            throw std::system_error(errno, std::system_category(), 
                    format("mq_open error for {}", name));
        }
        va_end(ap);
    } else {
        if ( (mqd = mq_open(name, oflag)) == (mqd_t) -1) {
            throw std::system_error(errno, std::system_category(), 
                    format("mq_open error for {}", name));
        }
    }
    return(mqd);
}

void Mq_close(mqd_t mqd) {
    if (mq_close(mqd) == -1) {
        throw std::system_error(errno, std::system_category(), "mq_close error");
    }
}


