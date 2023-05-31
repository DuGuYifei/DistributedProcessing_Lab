#include "monitor.h"
#include <stdio.h>

#define NUM_PRODUCERS 100

void *producer(void *arg) {
    Monitor *monitor = (Monitor *)arg;
    int product;

    for (product = 0; product < NUM_PRODUCERS; product++) {
        monitor_produce(monitor, product);
        printf("Producer inserted: %d\n", product);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
	Monitor *monitor = (Monitor *)arg;
	int i, product;

	for (i = 0; i < NUM_PRODUCERS; i++) {
		product = monitor_consume(monitor);
		printf("Consumer removed: %d\n", product);
	}

	pthread_exit(NULL);
}

int main(){
	pthread_t producer_thread, consumer_thread;
	Monitor monitor;
	monitor_init(&monitor);

	pthread_create(&producer_thread, NULL, producer, &monitor);
	pthread_create(&consumer_thread, NULL, consumer, &monitor);

	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);

	return 0;
}