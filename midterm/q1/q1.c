#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>


#define COUNT_THREAD 10
#define COUNT_KNIFE 4
#define COUNT_OVEN 3
#define COUNT_PEPPER_GRINDER 2
#define COUNT_POT 5

sem_t knife_mutex;
sem_t oven_mutex;
sem_t pepper_grinder_mutex;
sem_t pot_mutex;


void msleep(unsigned int tms) {
	usleep (tms * 1000);
}


void * chef (void * arg) {
	int id = *((int *)arg);
	int num;
	while (sem_getvalue (&oven_mutex, &num) == 0 && num == 0) 
		sem_wait (&oven_mutex);

	printf ("chef %d is acquiring an oven\n", id);

	sem_post (&oven_mutex);

	while (sem_getvalue (&knife_mutex, &num) == 0 && num == 0)
		sem_wait (&knife_mutex);

	printf ("chef %d is acquiring a knife\n", id);

	sem_post (&knife_mutex);

	printf ("chef %d is doing phase a\n", id);
	msleep (300);
	printf ("chef %d finished phase a\n", id);

	while (sem_getvalue (&pot_mutex, &num) == 0 && num == 0) 
		sem_wait (&pot_mutex);

	printf ("chef %d is acquiring a pot\n", id);

	sem_post (&pot_mutex);

	printf ("chef %d is doing phase c\n", id);
	msleep (200);
	printf ("chef %d finished phase c\n", id);

	while (sem_getvalue (&pepper_grinder_mutex, &num) == 0 && num == 0)
		sem_wait (&pepper_grinder_mutex);

	printf("chef %d is acquiring a pepper grinder\n", id);

	sem_post (&pepper_grinder_mutex);

	printf("chef %d is doing phase b\n", id);
	msleep(100);
	printf("chef %d finished phase b\n", id);
	
	pthread_exit (NULL);
}

int main () {
	pthread_t threads[COUNT_THREAD];
	int thread_id[COUNT_THREAD];

	sem_init (&knife_mutex, 0, 4);
	sem_init (&oven_mutex, 0, 3);
	sem_init (&pepper_grinder_mutex, 0, 2);
	sem_init (&pot_mutex, 0, 5);
	
	for (int i = 0; i < COUNT_THREAD; i++) {
		thread_id[i] = i;
		pthread_create (&threads[i], NULL, chef, (void *)&thread_id[i]);
	}
	

	for (int i = 0; i < COUNT_THREAD; i++) 
		pthread_join (threads[i], NULL);


	sem_destroy (&knife_mutex);
	sem_destroy (&oven_mutex);
	sem_destroy (&pepper_grinder_mutex);
	sem_destroy (&pot_mutex);
	
	return EXIT_SUCCESS;
}
