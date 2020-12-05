#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#define COUNT_THREAD 5

enum philosopher_state {
    Thinking,
    Eating,
    Hungry
};

// define forks as mutex locks
pthread_mutex_t forks[COUNT_THREAD];

// =============== new ======================
int p_states[COUNT_THREAD]; // state for each philosopher
// this is for creating mutual exclusion for accessing shared state
sem_t state_mutex;
sem_t s[COUNT_THREAD]; // a semaphore for each fork
// ========================================

typedef struct {
    volatile int run;
    int pid;
} thread_arg_t;

void test(int pid);  // <================== new
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

    // initialize forks and semaphores
    sem_init(&state_mutex, 0, 1);
    for (i = 0; i < COUNT_THREAD; i++) {
        pthread_mutex_init(&forks[i], NULL);
        sem_init(&s[i], 0, 1);
        p_states[i] = Thinking;
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
    int left = (pid + N - 1) % N;
    int right = (pid + 1) % N;
    while(arg->run)
    {
        think();

        // take fork process
        sem_wait(&state_mutex);
        p_states[pid] = Hungry;
        test(pid);  // check if I can eat
        sem_post(&state_mutex);
        sem_wait(&s[pid]);
        // ==================

        eat();

        // put the forks down
        sem_wait(&state_mutex);

        put_fork(pid);
        printf("pid: %d fork %d release\n", pid, pid);

        put_fork((pid + 1) % N);
        printf("pid: %d fork %d release\n", pid, (pid + 1) % N);

        p_states[pid] = Thinking;
        test(left); // ask left person if he wants to eat
        test(right); // ask right person if he wants to eat

        sem_post(&state_mutex);
        // ===================
    }
    pthread_exit(NULL);
}

/* Assuming the state_mutex has been acquired before
 * entering this function.
 * */
void test(int pid)
{
    int N = COUNT_THREAD;
    int left = (pid + N - 1) % N;
    int right = (pid + 1) % N;
    if (p_states[pid] == Hungry &&
            p_states[left] != Eating &&
            p_states[right] != Eating) {

        // Others can not take any fork because this philosopher and
        // only this philosopher has the lock (state_mutex).
        take_fork(pid);
        printf("pid: %d fork %d acquired\n", pid, pid);

        take_fork((pid + 1) % N);
        printf("pid: %d fork %d acquired\n", pid, (pid + 1) % N);

        p_states[pid] = Eating;
        sem_post(&s[pid]);
    }
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
