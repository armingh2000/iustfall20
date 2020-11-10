/* Operating Systems Course - Fall 2020
 * Iran University of Science and Technology
 *
 * The goal of this program is to calculate the summation of values in
 * the data directory in modulos 1e9+7 using multiple threads.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

#define M (1000000000+7)
#define MAX_FILE_PATH_LEN 128

typedef struct {
	int from;
	int up_to;
	int result;
} thread_arg_t;


int mod(long int num, int m)
{
	int tmp = num % m;
	if (tmp < 0) tmp += m;
	return tmp;
}

void *worker_func(void *_arg)
{
	thread_arg_t *arg = _arg;
	int from_file = arg->from;
	int up_to_file = arg->up_to;
	FILE *fd;
	char file_path[MAX_FILE_PATH_LEN];
	int input;
	long int sum = 0;

	// iterat over files in data directory
	for (int i = from_file; i < up_to_file; i++) {
		// open a file
		snprintf(file_path, MAX_FILE_PATH_LEN, "./data/%d.dat", i);
		fd = fopen(file_path, "r");

		// read data from file
		while (fscanf(fd, "%d\r\n", &input)) {
			sum += input;
			sum = mod(sum, M);
			// printf("input: %ld\n", input);
		}
	}

	// write result to a place where main knows how to access
        arg->result = sum;

	pthread_exit(NULL);
//	return (void *)0;
}

int main(int argc, char *argv[])
{
	const int count_threads = 10;
	int count_files = 100;

	int delta = count_files / count_threads;


	pthread_t threads[count_threads];
	thread_arg_t args[count_threads];
        int rc; // return code

	int i;
	int child_return;
	long int sum = 0;

	struct timespec before, after;
	long int total_time;

	// get time
	clock_gettime(CLOCK_MONOTONIC, &before);

	for (i = 0; i < count_threads; i++) {
		// what files this child is going to process
		args[i].from = i * delta;
		args[i].up_to = args[i].from + delta;

		rc = pthread_create(&threads[i], NULL, worker_func,
				(void *)(args+i));
		if (rc != 0) {
			printf("failed to create a thread\n");
		}

		printf("thread %d created files: %d up to %d\n", i,
				args[i].from, args[i].up_to);
	}

	// wait until threads do the job
	for (i = 0; i < count_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	for (i = 0; i < count_threads; i++) {
		child_return = args[i].result;
		// printf("read from pipe: %d\n", child_return);
		sum += child_return;
		sum = mod(sum, M);
	}

	// get after
	clock_gettime(CLOCK_MONOTONIC, &after);

	total_time = (after.tv_sec - before.tv_sec) * 1000;
	total_time += (after.tv_nsec - before.tv_nsec) / 1000000;

	printf("our result is %ld\n", sum);
	printf("duration %ld (ms)\n", total_time);
	return 0;
}
