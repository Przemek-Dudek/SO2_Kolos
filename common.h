#if !defined(_COMMON_H_)
#define _COMMON_H_
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

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
#endif