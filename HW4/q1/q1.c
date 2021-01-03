#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <stdlib.h>
#include <alloca.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/mman.h>
#include <wait.h>
#include <time.h>


#define PROC_COUNT 1 
#define SIZE 4096
#define NAME "queues"
#define QUEUE_LENGHT 20
#define PRIME_COUNT 1 


struct prime {
	int value;           // value of the prime number
	int counter;         // a counter to decrement (at the start counter = value)
};

struct shared_data {
	int counter;
	sem_t * mutexes;
	int ** queues_ctl; // add, remove, total
	struct prime *** queues;
	struct prime ** print_queue;
	sem_t add_mutex;
	int print_index;
};


int is_prime (int n) {
	for (int i = 2; i < n; i++)
		if (n % i == 0)
			return 0;
	return 1;
}

void add_to_print_queue (struct shared_data * sd, struct prime p) {
	sem_wait (&(sd -> add_mutex));

	sd -> print_queue[sd -> print_index] = &p;
	sd -> print_index++;

	sem_post (&(sd -> add_mutex));
}

void next_pu (struct shared_data * sd, struct prime p, int id) {
	printf("%d %d", p.value, p.counter);
	p.value += id;
	p.counter -= 1;
	sem_wait (&(sd -> add_mutex));

	int npu = id + 1;
	npu %= PROC_COUNT;

	while (sd -> queues_ctl[npu][2] == QUEUE_LENGHT);
	sd -> queues[npu][sd -> queues_ctl[npu][0]] = &p;
	sd -> queues_ctl[npu][2]++;
	sd -> queues_ctl[npu][0]++;
	sd -> queues_ctl[npu][0] %= QUEUE_LENGHT;

	sem_post (&(sd -> add_mutex));

}

void child_process (struct shared_data * sd, int id) {
	printf("hrereere\n");

	while (1) {
		sleep(1);
		if (sd -> counter > 0) {
			sem_wait (&(sd -> mutexes[id]));
			printf("child: id %d, add %d, remove %d, total %d, counter %d\n", id, sd -> queues_ctl[id][0], sd -> queues_ctl[id][1], sd -> queues_ctl[id][2], sd -> counter);
			fflush(stdout);

			if (sd -> queues_ctl[id][2] > 0) {
				struct prime p = *(sd -> queues[id][sd -> queues_ctl[id][1]]);
				if (p.counter == 0) {
					add_to_print_queue (sd, p);
				} else {
					next_pu (sd, p, id);
				}
				sd -> queues_ctl[id][2]--;
				sd -> queues_ctl[id][1]++;
				sd -> queues_ctl[id][1] %= QUEUE_LENGHT;
			}

			sem_post (&(sd -> mutexes[id]));
		}
	}

	exit(EXIT_SUCCESS);
}

void parent_process (struct shared_data * sd) {
	int current_number = 2, remainder = 0;

	while (1) {
		printf("parent: counter %d, remainder %d, add %d, remove %d, total %d\n", sd -> counter, remainder, sd -> queues_ctl[remainder][0], sd -> queues_ctl[remainder][1], sd -> queues_ctl[remainder][2]);
		if (sd -> counter < PRIME_COUNT) {
			if (is_prime (current_number)) {
				remainder = current_number % PRIME_COUNT;

				sem_wait (&(sd -> mutexes[remainder]));
				printf("%d\n", remainder);
				if (sd -> queues_ctl[remainder][2] < QUEUE_LENGHT ) {
					struct prime p;
					p.value = remainder;
					p.counter = remainder;
					(sd -> queues)[remainder][sd -> queues_ctl[remainder][0]] = &p;	

					sem_wait (&(sd -> add_mutex));
					sd -> queues_ctl[remainder][0]++;
					sem_post (&(sd -> add_mutex));

					sd -> queues_ctl[remainder][2]++;
					sd -> queues_ctl[remainder][0] %= QUEUE_LENGHT;

					printf ("parent: id %d, add %d, remove %d, total %d\n", remainder, sd -> queues_ctl[remainder][0], sd -> queues_ctl[remainder][1], sd -> queues_ctl[remainder][2]);
					fflush (stdout);
				}

				sem_post (&(sd -> mutexes[remainder]));
				sd -> counter++;
			}
			current_number++;
		} else {
			printf("elseeeeeeeeee");
			break;
		}

	}
}

int main (int argc, char * argv[]) {

	int shm_fd = shm_open (NAME, O_CREAT | O_RDWR, 0666);
	ftruncate (shm_fd, SIZE);

	sem_t * mutexes = (sem_t *) mmap (NULL, PROC_COUNT * sizeof(*mutexes), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	
	int ** queues_ctl = (int **) mmap (NULL, PROC_COUNT * sizeof(*queues_ctl), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	
	struct prime *** queues = (struct prime ***) mmap (NULL, PROC_COUNT * sizeof(*queues), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

	struct prime ** print_queue = (struct prime **) mmap (NULL, PRIME_COUNT * sizeof(*print_queue), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

	struct shared_data * sd = (struct shared_data *) mmap (NULL, sizeof(*sd), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	
	sd -> counter = 0;
	sd -> mutexes = mutexes;
	sd -> queues_ctl = queues_ctl;
	sd -> queues = queues;
	sd -> print_queue = print_queue;
	sem_init (&(sd -> add_mutex), 1, 1);
	sd -> print_index = 0;

	for (int i = 0; i < PROC_COUNT; i++) {
		sem_init (&(sd -> mutexes[i]), 1, 1);

		sd -> queues_ctl[i] = (int *) mmap (NULL, 3 * sizeof(*(sd -> queues_ctl[i])), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

		for (int j = 0; j < 3; j++) {
			//printf("%d", sd -> queues_ctl[i][j]);
			sprintf (&(sd -> queues_ctl[i][j]), "%d", 0);
			printf("%d", sd -> queues_ctl[i][j]);
		}

	//	sd -> queues[i] = (struct prime **) mmap (NULL, QUEUE_LENGHT * sizeof(*(sd -> queues[i])), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	}


	int fid;

	for (int i = 0; i < PROC_COUNT; i++) {
		if ((fid = fork()) == 0) ;
			//child_process (sd, i);

		else if (fid < 0) {
			fprintf (stderr, "fork error");

			return EXIT_FAILURE;
		}
	}

	//parent_process (sd);

	while (wait(NULL) > 0);

	printf("end\n");
	for (int i = 0; i < PROC_COUNT; i++) {
		sem_destroy (&(sd -> mutexes[i]));
	}

	sem_destroy(&(sd -> add_mutex));
	munmap (NAME, SIZE);

	return EXIT_SUCCESS;
}
