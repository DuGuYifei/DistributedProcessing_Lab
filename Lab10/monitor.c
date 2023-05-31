#include "monitor.h"


void monitor_init(Monitor *monitor) {
    monitor->in = 0;
    monitor->out = 0;
    semaphore_init(&monitor->mutex, 1);
    semaphore_init(&monitor->items, 0);
    semaphore_init(&monitor->spaces, BUFFER_SIZE);
}

void monitor_produce(Monitor *monitor, int product){
	semaphore_wait(&(monitor->spaces)); // wait there is space
	semaphore_wait(&(monitor->mutex));
	monitor->buffer[monitor->in] = product;
	monitor->in = (monitor->in + 1) % BUFFER_SIZE;
	semaphore_signal(&(monitor->mutex));
	semaphore_signal(&(monitor->items)); // signal there is a product
}

int monitor_consume(Monitor *monitor){
	int product;
	semaphore_wait(&(monitor->items)); // wait there is a product
	semaphore_wait(&(monitor->mutex));
	product = monitor->buffer[monitor->out];
	monitor->out = (monitor->out + 1) % BUFFER_SIZE;
	semaphore_signal(&(monitor->mutex));
	semaphore_signal(&(monitor->spaces)); // signal there is a space
	return product;
}
