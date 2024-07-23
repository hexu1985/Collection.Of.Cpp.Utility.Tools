#pragma once

#include <semaphore.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

sem_t*  Sem_open(const char* name, int oflag, .../* mode_t mode, unsigned int value */);
void    Sem_close(sem_t *);
void    Sem_unlink(const char *);
void    Sem_wait(sem_t* sem); 
int     Sem_trywait(sem_t* sem); 
void    Sem_post(sem_t* sem); 
void    Sem_getvalue(sem_t* sem, int* valp); 
