#include "shared_memory_object.hpp"

#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>

#include <system_error>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

SharedMemoryObject::SharedMemoryObject() noexcept {
}

SharedMemoryObject::SharedMemoryObject(const char* name, bool check_exists) {
    int flags = O_RDWR | O_CREAT;
    if (check_exists) {
        flags |= O_EXCL;
    }

    mode_t mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if ((fd_ = shm_open(name, flags, mode)) == -1) {
        throw std::system_error(errno, std::system_category(), 
                format("shm_open error for {}", name));
    }
}

SharedMemoryObject::~SharedMemoryObject() {
    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }
}

SharedMemoryObject::SharedMemoryObject(SharedMemoryObject&& other) {
    fd_ = other.fd_;
    other.fd_ = -1;
}

SharedMemoryObject& SharedMemoryObject::operator= (SharedMemoryObject&& other) {
    if (&other == this) {
        return *this;
    }

    if (fd_ >= 0) {
        close(fd_);
    }

    fd_ = other.fd_;
    other.fd_ = -1;
}

void SharedMemoryObject::truncate(size_t length) {
    if (ftruncate(fd_, length) == -1) {
        throw std::system_error(errno, std::system_category(), "ftruncate error");
    }
}

size_t SharedMemoryObject::size() const {
    struct stat stat;
	if (fstat(fd_, &stat) == -1) {
        throw std::system_error(errno, std::system_category(), "fstat error");
    }
    return stat.st_size;
}

int SharedMemoryObject::fileno() const {
    return fd_;
}

void SharedMemoryObject::swap(SharedMemoryObject& other) noexcept {
    using std::swap;
    swap(fd_, other.fd_);
}

bool SharedMemoryObject::remove(const char* name) noexcept {
    if (shm_unlink(name) == -1) {
        return false;
    }
    return true;
}

