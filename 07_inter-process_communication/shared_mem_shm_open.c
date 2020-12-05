//
// Created by Vahid on 11/28/2020.
//

// compiler flags: -lrt -pthread
// example: gcc shared_mem_shm_open.c -lrt -pthread -o out

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>


#define USE_SEMAPHORE 1


int main(void)
{
    // size of share memory object
    const int SIZE = 4096;

    // name of the shared memory object
    const char *name = "/SHM";

    // messages to be shared
    const char *msg1 = "Hello";
    const char *msg2 = "World!";

    // shared memory file descriptor
    int fd;

    // pointer to shared memory object
    char *ptr;

    // semaphore
    sem_t *mutex;
    if (USE_SEMAPHORE) {

        mutex = (sem_t *) mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

        sem_init(mutex, 1, 0);
    }


    // create producer and consumer processes
    int producer;
    if((producer = fork()) < 0)
    {
        printf("fork failed\n");
        exit(-1);
    }
    // producer process
    else if(producer != 0)
    {
        printf("Producer started ...\n");

        // create shared memory object
        fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        /* 0666 equals to (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH*/

        if (fd < 0)
        {
            printf("Error in producer\n");
        }

        // config the size of shared memory
        ftruncate(fd, SIZE);

        // map the shared memory object to the memory
        ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (ptr == MAP_FAILED)
            printf("failed in mapping\n");

        // write to shared memory object
        sprintf(ptr, "%s", msg1);
        ptr += strlen(msg1);
        sprintf(ptr, "%s", msg2);
        ptr += strlen(msg2);

        if (USE_SEMAPHORE) {
            sem_post(mutex);
        }


    }
    // consumer process
    else
    {
        if (USE_SEMAPHORE) {
            sem_wait(mutex);
        } else{
            sleep(5);
        }
        printf("Consumer started ...\n");

        // open the shared memory object
        fd = shm_open(name, O_RDONLY, 06666);

        if (fd < 0)
        {
            printf("Error in consumer\n");
        }

        // map the shared memory object object to the memory
        ptr = (char *) mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED)
            printf("failed in mapping\n");

        // read from the shared memory object
        printf("msg: %s\n", (char *)ptr);

        // remove shared memory
        shm_unlink(name);
    }



    if(producer != 0)
        wait(NULL);
    return 0;
}