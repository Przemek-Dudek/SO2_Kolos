#include "common.h"

int comp(const void *a, const void *b)
{
    u_int32_t a_int = *(u_int32_t*)a;
    u_int32_t b_int = *(u_int32_t*)b;

    if(a_int < b_int) return -1;

    if(a_int == b_int) return 0;

    return 1;
}

int main()
{
    int fd = shm_open("/msg_data", O_CREAT | O_RDWR, 0600);
    ftruncate(fd, sizeof(struct data_t));
    struct data_t *pdata = (struct data_t*) mmap(NULL, sizeof(struct data_t*), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_init(&pdata->sem_1,1,0);
    sem_init(&pdata->sem_2,1,0);


    sem_wait(&pdata->sem_1);

    printf("size = %d\n", pdata->size);

    int wsk = shm_open("/msg_tab", O_CREAT | O_RDWR, 0600);
    ftruncate(wsk, sizeof(u_int32_t)*pdata->size);
    u_int32_t *tab = (u_int32_t*) mmap(NULL, sizeof(u_int32_t)*pdata->size, PROT_READ | PROT_WRITE, MAP_SHARED, wsk, 0);

    sem_post(&pdata->sem_2);

    sem_wait(&pdata->sem_1);

    qsort(tab, pdata->size, sizeof(u_int32_t), comp);
    for(int i = 0; i < pdata->size; i++) {
        printf("%d ", *(tab+i));
    }

    sem_post(&pdata->sem_2);
    sem_post(&pdata->sem_1);

    munmap(tab, sizeof(u_int32_t)*pdata->size);

    shm_unlink("/msg_tab");

    return 0;
}