#include "error.hpp"

#include <stdarg.h>
#include <errno.h>
#include <stdexcept>
#include <sstream>

void error_file_line(const char* file, unsigned int line, const char* format, ...)
{
    const int buf_size = 1024;
    char    buf[buf_size] = {'\0'};
    va_list ap;

    va_start (ap, format);
    vsnprintf(buf, buf_size, format, ap);
    va_end (ap);
    std::ostringstream os;
    os << "[" << file << ":" << line << "] " << buf;
    throw std::runtime_error(os.str());
}

