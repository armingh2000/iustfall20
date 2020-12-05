#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#define COUNT_THREAD 5

// define forks as mutex locks
pthread_mutex_t forks[COUNT_THREAD];

typedef struct {
    volatile int run;
    int pid;
} thread_arg_t;

void think(void);
void eat(void);
void take_fork(int fid);
void put_fork(int fid);
void *philosophore(void *arg);

int main(int argc, char *argv[])
{
    int i;
    pthread_t threads[COUNT_THREAD];
    thread_arg_t args[COUNT_THREAD];

    srand(77);

    // initialize forks
    for (i = 0; i < COUNT_THREAD; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // start threads
    for (i = 0; i < COUNT_THREAD; i++) {
        args[i].pid = i;
        args[i].run = 1;
        pthread_create(&threads[i], NULL, philosophore, (void *)&args[i]);
    }

    // wait for threads to finish
    for (i = 0; i < COUNT_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

void *philosophore(void *_arg)
{
    thread_arg_t *arg = _arg;
    int pid = arg->pid;
    int N = COUNT_THREAD;
    while(arg->run)
    {
        think();

        take_fork(pid);
        printf("pid: %d fork %d acquired\n", pid, pid);

        take_fork((pid + 1) % N);
        printf("pid: %d fork %d acquired\n", pid, (pid + 1) % N);

        eat();

        put_fork(pid);
        printf("pid: %d fork %d release\n", pid, pid);

        put_fork((pid + 1) % N);
        printf("pid: %d fork %d release\n", pid, (pid + 1) % N);
    }
    pthread_exit(NULL);
}

void take_fork(int fid)
{
    struct timespec tim, rmtp;
    tim.tv_sec = 1;
    tim.tv_nsec = 500000;
    pthread_mutex_lock(&forks[fid]);
    nanosleep(&tim, &rmtp);
}

void put_fork(int fid)
{
    pthread_mutex_unlock(&forks[fid]);
}

void think(void)
{
    // function details does not matter
    int jitter = rand() % 5;
    sleep(jitter);
}

void eat(void)
{
    // function details does not matter
    sleep(1);
}
