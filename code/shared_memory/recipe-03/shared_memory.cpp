#include "shared_memory.hpp"

#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>

#include <system_error>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

namespace {

int Shm_open(const char *name, int oflag, mode_t mode) {
    int fd;
    if ((fd = shm_open(name, oflag, mode)) == -1) {
        throw std::system_error(errno, std::system_category(), 
                format("shm_open error for {}", name));
    }
    return fd;
}

const mode_t FILE_MODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

}   // namespace

SharedMemory::SharedMemory() noexcept {
}

SharedMemory::SharedMemory(int fd): fd_(fd) {
}

SharedMemory::~SharedMemory() {
    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }
}

SharedMemory::SharedMemory(SharedMemory&& other) {
    fd_ = other.fd_;
    other.fd_ = -1;
}

SharedMemory& SharedMemory::operator= (SharedMemory&& other) {
    if (&other == this) {
        return *this;
    }

    if (fd_ >= 0) {
        close(fd_);
    }

    fd_ = other.fd_;
    other.fd_ = -1;
    return *this;
}

void SharedMemory::truncate(size_t length) {
    if (ftruncate(fd_, length) == -1) {
        throw std::system_error(errno, std::system_category(), "ftruncate error");
    }
}

size_t SharedMemory::size() const {
    struct stat stat;
	if (fstat(fd_, &stat) == -1) {
        throw std::system_error(errno, std::system_category(), "fstat error");
    }
    return stat.st_size;
}

int SharedMemory::fileno() const {
    return fd_;
}

void SharedMemory::swap(SharedMemory& other) noexcept {
    using std::swap;
    swap(fd_, other.fd_);
}

void* SharedMemory::map(size_t length, bool readonly, long offset) {
	void* ptr;
    int prot = readonly ? (PROT_READ) : (PROT_READ | PROT_WRITE);
	if ((ptr = mmap(NULL, length, prot, MAP_SHARED, fd_, offset)) == MAP_FAILED) {
        throw std::system_error(errno, std::system_category(), "mmap error");
    }
	return ptr;
}

bool SharedMemory::exists(const char* name) noexcept {
    if (shm_open(name, O_RDONLY, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
       return false;
    else
       return true; 
}

bool SharedMemory::remove(const char* name) noexcept {
    if (shm_unlink(name) == -1) {
        return false;
    }
    return true;
}

void SharedMemory::unmap(void* addr, size_t length) noexcept {
    munmap(addr, length);
}

SharedMemory SharedMemory::create_only(const char* name) {
    int flags = (O_RDWR | O_CREAT | O_EXCL);
    return SharedMemory(Shm_open(name, flags, FILE_MODE));
}

SharedMemory SharedMemory::open_or_create(const char* name) {
    int flags = (O_RDWR | O_CREAT);
    return SharedMemory(Shm_open(name, flags, FILE_MODE));
}

SharedMemory SharedMemory::open_read_write(const char* name) {
    return SharedMemory(Shm_open(name, O_RDWR, 0));
}

SharedMemory SharedMemory::open_read_only(const char* name) {
    return SharedMemory(Shm_open(name, O_RDONLY, 0));
}

