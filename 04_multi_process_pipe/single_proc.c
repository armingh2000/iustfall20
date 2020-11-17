/* Operating Systems Course - Fall 2020
 * Iran University of Science and Technology
 *
 * The goal of this program is to calculate the summation of values in
 * the data directory in modulo 1e9+7 using only one process.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define M (1000000000+7)
#define MAX_FILE_PATH_LEN 128


int mod(int num, int m)
{
	int tmp = num % m;
	if (tmp < 0) tmp += m;
	return tmp;
}

int main(int argc, char *argv[])
{
	int count_files = 100;
	FILE *fd;
	char file_path[MAX_FILE_PATH_LEN];
	long int input;
	long int sum = 0;

	struct timespec before, after;
	long int total_time;

	// get time
	clock_gettime(CLOCK_MONOTONIC, &before);

	// iterat over files in data directory
	for (int i = 0; i < count_files; i++) {
		// open a file
		snprintf(file_path, MAX_FILE_PATH_LEN, "./data/%d.dat", i);
		fd = fopen(file_path, "r");

		// read data from file
		while (fscanf(fd, "%ld\r\n", &input)) {
			sum += input;
			sum = mod(sum, M);
			// printf("input: %ld\n", input);
		}
	}

	// get after
	clock_gettime(CLOCK_MONOTONIC, &after);

	total_time = (after.tv_sec - before.tv_sec) * 1000;
	total_time += (after.tv_nsec - before.tv_nsec) / 1000000;

	printf("our result is %ld\n", sum  % M);
	printf("duration %ld (ms)\n", total_time);
	return 0;
}
