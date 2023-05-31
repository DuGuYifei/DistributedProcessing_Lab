#include "semaphore.h"

void semaphore_init(semaphore_t *s, int value){
	s->count = value;
	pthread_mutex_init(&s->mutex, NULL);
	pthread_cond_init(&s->condition, NULL);
}

void semaphore_wait(semaphore_t *s){
	pthread_mutex_lock(&s->mutex);
	s->count--;
	while(s->count < 0){
		pthread_cond_wait(&s->condition, &s->mutex);
	}
	pthread_mutex_unlock(&s->mutex);
}

void semaphore_signal(semaphore_t *s){
	pthread_mutex_lock(&s->mutex);
	s->count++;
	pthread_cond_signal(&s->condition);
	pthread_mutex_unlock(&s->mutex);
}