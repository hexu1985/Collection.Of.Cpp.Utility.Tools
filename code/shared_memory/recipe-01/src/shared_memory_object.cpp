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

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

namespace {

int Shm_open(const char* name, int oflag, mode_t mode) {
    int fd;

    if ((fd = shm_open(name, oflag, mode)) == -1) {
        throw std::system_error(errno, std::system_category(), 
                format("shm_open error for {}", name));
    }
    return fd;
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

void Fstat(int fd, struct stat *statbuf) {
	if (fstat(fd, statbuf) == -1) {
        throw std::system_error(errno, std::system_category(), "fstat error");
    }
}

}   // namespace

SharedMemoryObject::SharedMemoryObject(const char* name, bool readonly) {
    try {
        int oflag = readonly ? O_RDONLY : O_RDWR;
        fd_ = Shm_open(name, oflag, FILE_MODE);

        size_t length = get_size();
        int mflag = readonly ? PROT_READ : (PROT_READ | PROT_WRITE);
        ptr_ = Mmap(NULL, length, mflag, MAP_SHARED, fd_, 0);
    } catch (const std::exception& e) {
        if (fd_ >= 0) close(fd_);
        throw;
    }
}

SharedMemoryObject::~SharedMemoryObject() {
    clear();
}

SharedMemoryObject::SharedMemoryObject(SharedMemoryObject&& other): fd_(other.fd_), ptr_(other.ptr_) {
    other.fd_ = -1;
    other.ptr_ = nullptr;
}

SharedMemoryObject& SharedMemoryObject::operator= (SharedMemoryObject&& other) {
    if (&other == this) {
        return *this;
    }

    clear();
    fd_ = other.fd_;
    ptr_ = other.ptr_;
    
    other.fd_ = -1;
    other.ptr_ = nullptr;

    return *this;
}

void SharedMemoryObject::truncate(size_t length) {
    if (ftruncate(fd_, length) == -1) {
        throw std::system_error(errno, std::system_category(), "ftruncate error");
    }
}

size_t SharedMemoryObject::get_size() const {
    struct stat stat;
    Fstat(fd_, &stat);
    return stat.st_size;
}

void* SharedMemoryObject::get_address() const {
    return ptr_;
}

SharedMemoryObject::SharedMemoryObject(int fd, void* ptr): fd_(fd), ptr_(ptr) {
}

void SharedMemoryObject::clear() {
    if (fd_ < 0) return;

    if (ptr_) {
        size_t length = get_size();
        munmap(ptr_, length);
        ptr_ = nullptr;
    }
    close(fd_);
    fd_ = -1;
}

SharedMemoryObject SharedMemoryObject::create(const char* name, size_t length, bool check_exists) {
    int fd = -1;
    try {
        int oflag = O_RDWR | O_CREAT;
        if (check_exists) {
            oflag |= O_EXCL;
        }

        fd = Shm_open(name, oflag, FILE_MODE);
        Ftruncate(fd, length);
        void* ptr = Mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        return SharedMemoryObject(fd, ptr);
    } catch (const std::exception& e) {
        if (fd >= 0) {
            close(fd);
        }
        throw;
    }
}

bool SharedMemoryObject::remove(const char* name) noexcept {
    if (shm_unlink(name) == -1) {
        return false;
    }
    return true;
}

