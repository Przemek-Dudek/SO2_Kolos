#include "common.h"

int main() {
    int fd = shm_open("/msg_data", O_RDWR, 0600);
    err(fd == -1, "shm_open");

    struct data_t* pdata = (struct data_t*)mmap(NULL, sizeof(struct data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    err(pdata == NULL, "mmap");

    srand(time(NULL));
    int my_id = rand() % 1000;

    FILE *f = fopen("payload.txt", "r");
    if(f == NULL) {
        printf("Odczyt nieudany");
        return 2;
    }

    int size = 0;

    for(int tmp; fscanf(f, "%d", &tmp) == 1; size++);

    fseek(f, 0, SEEK_SET);

    pdata->size = size;

    sem_post(&pdata->sem_1);
    sem_wait(&pdata->sem_2);

    int pay = shm_open("/msg_tab", O_CREAT | O_RDWR, 0600);
    u_int32_t *tab = (u_int32_t*) mmap(NULL, sizeof(u_int32_t)*pdata->size, PROT_READ | PROT_WRITE, MAP_SHARED, pay, 0);

    for(int i = 0; fscanf(f, "%d", tab+i) == 1; i++) {
        printf("%d ", *(tab+i));
    }

    printf("\n");

    fclose(f);

    pdata->id = my_id;

    sem_post(&pdata->sem_1);

    sem_wait(&pdata->sem_2);

    printf("size = %d\n", pdata->size);

    f = fopen("payload_sorted.txt", "w");
    if(f == NULL) {
        printf("Zapis nieudany");
        return 2;
    }

    for(int i = 0; i < pdata->size; i++) fprintf(f, "%d\n", *(tab+i));

    fclose(f);

    sem_post(&pdata->sem_1);

    munmap(tab, sizeof(u_int32_t)*pdata->size);

    shm_unlink("/msg_tab");

    munmap(pdata, sizeof(struct data_t));
    close(fd);

    return 0;
}
