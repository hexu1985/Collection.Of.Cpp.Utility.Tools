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

SharedMemoryObject::SharedMemoryObject() noexcept {
}

SharedMemoryObject::SharedMemoryObject(int fd): fd_(fd) {
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
    return *this;
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

void* SharedMemoryObject::map(size_t length, bool readonly, long offset) {
	void* ptr;
    int prot = readonly ? (PROT_READ) : (PROT_READ | PROT_WRITE);
	if ((ptr = mmap(NULL, length, prot, MAP_SHARED, fd_, offset)) == MAP_FAILED) {
        throw std::system_error(errno, std::system_category(), "mmap error");
    }
	return ptr;
}

bool SharedMemoryObject::exists(const char* name) noexcept {
    if (shm_open(name, O_RDONLY, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
       return false;
    else
       return true; 
}

bool SharedMemoryObject::remove(const char* name) noexcept {
    if (shm_unlink(name) == -1) {
        return false;
    }
    return true;
}

void SharedMemoryObject::unmap(void* addr, size_t length) noexcept {
    munmap(addr, length);
}

SharedMemoryObject SharedMemoryObject::create_only(const char* name) {
    int flags = (O_RDWR | O_CREAT | O_EXCL);
    return SharedMemoryObject(Shm_open(name, flags, FILE_MODE));
}

SharedMemoryObject SharedMemoryObject::open_or_create(const char* name) {
    int flags = (O_RDWR | O_CREAT);
    return SharedMemoryObject(Shm_open(name, flags, FILE_MODE));
}

SharedMemoryObject SharedMemoryObject::open_read_write(const char* name) {
    return SharedMemoryObject(Shm_open(name, O_RDWR, 0));
}

SharedMemoryObject SharedMemoryObject::open_read_only(const char* name) {
    return SharedMemoryObject(Shm_open(name, O_RDONLY, 0));
}

