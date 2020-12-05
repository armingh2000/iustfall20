#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define COUNT_THREAD 2

#define BOTTOM 0
#define LEFT 1
#define CENTER 2
#define RIGHT 3
#define TOP 5

#define Bottom_to_Up 0
#define Left_to_Right 1

// Shared resources =========
int cars_state[COUNT_THREAD];
int car_dir[COUNT_THREAD];
pthread_mutex_t cars_lock;

pthread_mutex_t tlight_mu;
int trafic_light;
pthread_cond_t BtoU;
pthread_cond_t LtoR;
// ==========================


int check_for_accident(int id);
void wait_for_traffic_light(int condition_id);

void *drive(void *arg)
{
	int id = (int)arg;
	int run = 1;

	while (run) {
		switch (cars_state[id]) {
			case BOTTOM:
				wait_for_traffic_light(BOTTOM);
				pthread_mutex_lock(&cars_lock);
				cars_state[id] = CENTER;
				if (check_for_accident(id)) {
					run = 0;
				}
				pthread_mutex_unlock(&cars_lock);
				break;
			case LEFT:
				wait_for_traffic_light(LEFT);
				pthread_mutex_lock(&cars_lock);
				cars_state[id] = CENTER;
				if (check_for_accident(id)) {
					run = 0;
				}
				pthread_mutex_unlock(&cars_lock);
				break;
			case TOP:
				pthread_mutex_lock(&cars_lock);
				cars_state[id] = LEFT;
				car_dir[id] = LEFT;
				pthread_mutex_unlock(&cars_lock);
				printf("car: %d success\n", id);
				break;
			case RIGHT:
				pthread_mutex_lock(&cars_lock);
				cars_state[id] = BOTTOM;
				car_dir[id] = BOTTOM;
				pthread_mutex_unlock(&cars_lock);
				printf("car: %d success\n", id);
			case CENTER:
				pthread_mutex_lock(&cars_lock);
				if (check_for_accident(id)) {
					run = 0;
				}
				if (car_dir[id] == BOTTOM) {
					cars_state[id] = TOP;
				} else {
					cars_state[id] = RIGHT;
				}
				pthread_mutex_unlock(&cars_lock);
				break;
		};
		if (run && cars_state[id] != CENTER)
			sleep(1); // wait for transition
	}
	pthread_exit(NULL);
}


int _tlight_run;
void *traffic_light_thread(void *arg)
{
	while (_tlight_run)
	{
		pthread_mutex_lock(&tlight_mu);
		if (trafic_light == Bottom_to_Up) {
			trafic_light = Left_to_Right;
			pthread_mutex_unlock(&tlight_mu);
			pthread_cond_signal(&LtoR);
			printf("traffic light: Left to Right\n");
		} else {
			trafic_light = Bottom_to_Up;
			pthread_mutex_unlock(&tlight_mu);
			pthread_cond_signal(&BtoU);
			printf("traffic light: Bottom to Up\n");
		}
		sleep(1);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[COUNT_THREAD];
	pthread_t tl_thread;
	int i;

	pthread_cond_init(&BtoU, NULL);
	pthread_cond_init(&LtoR, NULL);

	pthread_mutex_init(&tlight_mu, NULL);
	pthread_mutex_init(&cars_lock, NULL);
	_tlight_run = 1;

	trafic_light = Left_to_Right;

	for (i = 0; i < COUNT_THREAD; i++) {
		cars_state[i] =  i % 2;
		car_dir[i] = cars_state[i];
		pthread_create(&threads[i], NULL, drive, (void *)i);
	}
	printf("%d cars are in the streets\n", COUNT_THREAD);

	sleep(1);

	pthread_create(&tl_thread, NULL, traffic_light_thread, NULL);


	for (i = 0; i < COUNT_THREAD; i++) {
		pthread_join(threads[i], NULL);
	}
	_tlight_run = 0;
	pthread_join(tl_thread, NULL);
	return 0;
}


void wait_for_traffic_light(int condition_id)
{
	pthread_mutex_lock(&tlight_mu);
	if (condition_id == BOTTOM) {
		while (trafic_light != Bottom_to_Up) {
			pthread_cond_wait(&BtoU, &tlight_mu);
		}
	} else {
		while(trafic_light != Left_to_Right) {
			pthread_cond_wait(&LtoR, &tlight_mu);
		}
	}
	pthread_mutex_unlock(&tlight_mu);
	return;
}


int check_for_accident(int id)
{
	// should be locked before comming here
	int i;
	if (cars_state[id] == CENTER) {
		for (i = 0; i < COUNT_THREAD; i++) {
			if (i == id) continue;
			if (cars_state[i] == CENTER) {
				printf("accident: %d %d\n", i, id);
				return 1;
			}
		}

	}
	return 0;
}
