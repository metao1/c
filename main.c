#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <zconf.h>

#define SEMPERM 0666

int main() {
    sem_t *sem;
    int retval;
    int i, id, err, nloop = 25, count_;
    struct buffer {
        int counter;
        char content[nloop];
    };
    struct buffer *buff = malloc(sizeof(struct buffer));
    id = shmget(IPC_PRIVATE, sizeof(sem), SEMPERM);
    count_ = shmget(IPC_PRIVATE, sizeof(int), SEMPERM);

    if (id == -1) { perror("Error creating shared mem\n"); }
    if (count_ == -1) { perror("Error creating shared mem\n"); }
    sem = (sem_t *) shmat(id, (void *) 0, 0);
    buff = (struct buffer *) shmat(count_, (void *) 0, 0);
    retval = sem_init(sem, 1, 1);
    //opening the file
    FILE *fp = fopen("file.txt", "w");
    if (fp == NULL) {
        perror("Error creating the file\n");
        exit(-1);
    }

    //read(fp,)
    for (i = 0; i < nloop; i++) {
        if (fork() == 0) {
            sem_wait(sem);
            buff->content[i] = (char) ('a' + buff->counter);
            fprintf(fp, "%c\n", buff->content[i]);
            printf("Child working %d.\n", buff->counter++);
            sem_post(sem);
            exit(0);
        }
    }
    sem_wait(sem);
    for (i = 0; i < nloop; i++) {
        printf("Parent working %d:%c.\n", buff->counter, buff->content[i]);
    }
    sleep(1);
    sem_post(sem);
    if (retval != 0) {
        perror("error sem init\n");
        exit(3);
    }
    free(buff);
    err = shmctl(id, IPC_RMID, 0);
    if (err == 0) {
        perror("Error shared mem deallocation\n");
    }
    err = shmctl(count_, IPC_RMID, 0);
    if (err == 0) {
        perror("Error shared mem deallocation\n");
    }
    err = fclose(fp);
    if (err == 0) {
        perror("Error closing the file.\n");
    }
    return 0;
}