#pragma once

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* 4Posix shared memory */
int     Shm_open(const char* name, int oflag, mode_t mode);
void    Shm_unlink(const char* name);

void    Ftruncate(int fd, off_t length);
void*   Mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
void    Munmap(void* addr, size_t length);

void    Fstat(int fd, struct stat *statbuf);

void    Close(int fd);
