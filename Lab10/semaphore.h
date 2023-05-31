#include "pthread.h"
typedef struct semaphore semaphore_t;
struct semaphore {
	int count;
	pthread_mutex_t mutex;
	pthread_cond_t condition;
};

void semaphore_init(semaphore_t *s, int i);

void semaphore_wait(semaphore_t *s);

void semaphore_signal(semaphore_t *s);