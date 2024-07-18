#include "posix_api.hpp"
#include <errno.h>
#include <system_error>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

int Shm_open(const char* name, int oflag, mode_t mode) {
    int fd;

    if ((fd = shm_open(name, oflag, mode)) == -1) {
        throw std::system_error(errno, std::system_category(), 
                format("shm_open error for {}", name));
    }
    return fd;
}

void Shm_unlink(const char* name) {
    if (shm_unlink(name) == -1) {
        throw std::system_error(errno, std::system_category(), "shm_unlink error");
    }
}

void Ftruncate(int fd, off_t length) {
    if (ftruncate(fd, length) == -1) {
        throw std::system_error(errno, std::system_category(), "ftruncate error");
    }
}

void* Mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
	void* ptr;

	if ((ptr = mmap(addr, length, prot, flags, fd, offset)) == MAP_FAILED) {
        throw std::system_error(errno, std::system_category(), "mmap error");
    }
	return ptr;
}

void Munmap(void* addr, size_t length) {
	if (munmap(addr, length) == -1) {
        throw std::system_error(errno, std::system_category(), "munmap error");
    }
}

void Fstat(int fd, struct stat *statbuf) {
	if (fstat(fd, statbuf) == -1) {
        throw std::system_error(errno, std::system_category(), "fstat error");
    }
}

void Close(int fd) {
	if (close(fd) == -1) {
        throw std::system_error(errno, std::system_category(), "close error");
    }
}
