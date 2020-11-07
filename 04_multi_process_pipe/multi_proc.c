/* Operating Systems Course - Fall 2020
 * Iran University of Science and Technology
 *
 * The goal of this program is to calculate the summation of values in
 * the data directory in modulo 1e9+7 using multiple processes and pipes.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define M (1000000000+7)
#define MAX_FILE_PATH_LEN 128


int mod(long int num, int m)
{
	int tmp = num % m;
	if (tmp < 0) tmp += m;
	return tmp;
}

int worker_func(int from_file, int up_to_file)
{
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
	return sum;
}

int main(int argc, char *argv[])
{
	int count_child = 10;
	int count_files = 100;

	int delta = count_files / count_child;
	int from, up_to;

	pid_t fork_id;
	int pipe_fd[2];
	int result_pipe[count_child];

	int i;
	int child_return;
	long int sum = 0;

	struct timespec before, after;
	long int total_time;

	// get time
	clock_gettime(CLOCK_MONOTONIC, &before);

	for (i = 0; i < count_child; i++) {
		// what files this child is going to process
		from = i * delta;
		up_to = from + delta;

		// open a communication mechanism
		pipe(pipe_fd);

		// create a child
		fork_id = fork();
		if (fork_id < 0) {
			printf("forking failed!\n");
			// left children (if any)
			return -1;
		} else if (fork_id == 0) {
			// child

			// close read side of the pipe
			close(pipe_fd[0]);

			// let the child work
			child_return = worker_func(from, up_to);

			// send result to the parent
			write(pipe_fd[1], &child_return, sizeof(child_return));

			// printf("child says: %d\n", child_return);
			return 0; // child exits normaly
		}

		// close write end of the pipe
		close(pipe_fd[1]);
		result_pipe[i] = pipe_fd[0];
		printf("child %d created files: %d up to %d\n", i, from, up_to);
	}

	// wait until children do the job
	for (i = 0; i < count_child; i++) {
		wait(NULL);
	}

	for (i = 0; i < count_child; i++) {
		read(result_pipe[i], &child_return, sizeof(child_return));
		// printf("read from pipe: %d\n", child_return);
		sum += child_return;
		sum = mod(sum, M);
		close(result_pipe[i]);
	}

	// get after
	clock_gettime(CLOCK_MONOTONIC, &after);

	total_time = (after.tv_sec - before.tv_sec) * 1000;
	total_time += (after.tv_nsec - before.tv_nsec) / 1000000;

	printf("our result is %ld\n", sum);
	printf("duration %ld (ms)\n", total_time);
	return 0;
}
