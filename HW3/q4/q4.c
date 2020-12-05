#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


#define M 10000

struct task {
    int id;            // id of this task
    int value;         // value of this task
    int atime;         // the arrival time of the task
    int unit_count;    // number of units
    int *unit_id;      // list of units which task will be assigned to
    
    int current_unit_index; 
};

pthread_mutex_t lock_unit_0;
pthread_mutex_t lock_unit_1;
pthread_mutex_t lock_unit_2;
pthread_mutex_t lock_unit_3;
pthread_mutex_t lock_unit_4;


void * unit_0 (void * arg) {
	pthread_mutex_lock(&lock_unit_0);
	struct task * current_task = (struct task *)arg;
	
	current_task -> value = ((current_task -> value) + 7) % M;
	pthread_mutex_unlock(&lock_unit_0);

	pthread_exit(NULL);
}

void * unit_1 (void * arg) {
	pthread_mutex_lock(&lock_unit_1);
	struct task * current_task = (struct task *)arg;

	current_task -> value = ((current_task -> value) * 2) % M;
	pthread_mutex_unlock(&lock_unit_1);

	pthread_exit(NULL);
}

void * unit_2 (void * arg) {
	pthread_mutex_lock(&lock_unit_2);
	struct task * current_task = (struct task *)arg;

	current_task -> value = (int)pow(current_task -> value, 5) % M;
	pthread_mutex_unlock(&lock_unit_2);

	pthread_exit(NULL);
}
void * unit_3 (void * arg){
	pthread_mutex_lock(&lock_unit_3);
	struct task * current_task = (struct task *)arg;

	current_task -> value = ((current_task -> value) - 19) % M;
	pthread_mutex_unlock(&lock_unit_3);

	pthread_exit(NULL);
}

void * unit_4 (void * arg) {
	pthread_mutex_lock(&lock_unit_4);
	struct task * current_task = (struct task *)arg;

	printf("tid: %d value: %d\n", current_task -> id, current_task -> value);
	pthread_mutex_unlock(&lock_unit_4);

	pthread_exit(NULL);
}

void * handle_task (void * arg) {
	struct task * current_task = (struct task *)arg;
	current_task -> current_unit_index = 0;
	
	int current_unit;

	pthread_t ptid;
	while ((current_task -> current_unit_index) < (current_task -> unit_count)) {
		current_unit = (current_task -> unit_id)[current_task -> current_unit_index];
		
		switch (current_unit) {
			case 0:
				;
				pthread_create(&ptid, NULL, &unit_0, (void *)current_task);
				break;
			
			case 1:
				;
				pthread_create(&ptid, NULL, &unit_1, (void *)current_task);
				break;

			case 2:
				;
				pthread_create(&ptid, NULL, &unit_2, (void *)current_task);
				break;

			case 3:
				;
				pthread_create(&ptid, NULL, &unit_3, (void *)current_task);
				break;

			case 4:
				;
				pthread_create(&ptid, NULL, &unit_4, (void *)current_task);
				break;
		}

		pthread_join(ptid, NULL);
		(current_task -> current_unit_index) ++;
	}
	
	free(current_task -> unit_id);
	free(current_task);
	pthread_exit(NULL);
}

int main (int argc, char * argv[]) {
	pthread_mutex_init(&lock_unit_0, NULL);
	pthread_mutex_init(&lock_unit_1, NULL);
	pthread_mutex_init(&lock_unit_2, NULL);
	pthread_mutex_init(&lock_unit_3, NULL);
	pthread_mutex_init(&lock_unit_4, NULL);

	char * file_name = argv[1];
	
	FILE * fp;
	fp = fopen(file_name, "r");

	int task_id, task_value, unit_count;

	pthread_t * threads = malloc(sizeof(*threads));
	int number_of_threads = 0;
	
	while (fscanf(fp, "%d %d %d", &task_id, &task_value, &unit_count) == 3) {
		struct task * next_task = malloc(sizeof(*next_task));
		int * units = malloc(unit_count * sizeof(*units));
		number_of_threads++;
		threads = realloc(threads, number_of_threads * sizeof(*threads));
		
		for (int i = 0; i < unit_count; i++) {
			fscanf(fp, "%d", &units[i]);
		}
		
		next_task -> id = task_id;
		next_task -> value = task_value;
		next_task -> unit_count = unit_count;
		next_task -> unit_id = units;

		pthread_create(&threads[number_of_threads - 1], NULL, &handle_task, (void *)next_task);
	}

	for (int i = 0; i < number_of_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	pthread_mutex_destroy(&lock_unit_0);
	pthread_mutex_destroy(&lock_unit_1);
	pthread_mutex_destroy(&lock_unit_2);
	pthread_mutex_destroy(&lock_unit_3);
	pthread_mutex_destroy(&lock_unit_4);

	fclose(fp);

	return 0;
}
