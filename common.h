#if !defined(_COMMON_H_)
#define _COMMON_H_
#include <stdio.h>
#include <semaphore.h> //sem*
#include <sys/mman.h> // mmap, munmap, shm_open, shm_unlink
#include <fcntl.h> // O_*
#include <stdlib.h> // exit
#include <unistd.h> // close, ftruncate
#include <string.h> // strcasecmp
#include <time.h> // time

struct data_t {
    sem_t sem_1;
    sem_t sem_2;
    
    int id;
    u_int32_t size;
};

static void err(int c, const char* msg) {
    if (!c)
        return;
    perror(msg);
    exit(1);
}
#endif // _COMMON_H_