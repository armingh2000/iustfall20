#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#define CHILD_COUNT 8

sem_t count_mutex;
int count;
int queue[100];
int add_at = 0;
int remove_at = 0;

int main() {
	sem_init (&count_mutex, 1, CHILD_COUNT + 1);

	for (int i = 0; i < CHILD_COUNT; i++) 
		if (fork() == 0) {
			
			exit (EXIT_SUCCESS);
		}

	int current_num = 1000000000;
	int num;

	while (sem_getvalue (&count_mutex, &num) == 0 && num == 0)
		sem_wait (&count_mutex);
	



	return EXIT_SUCCESS;
}
