#include <pthread.h>
#include "semaphore.h"
#define BUFFER_SIZE 10


typedef struct Monitor Monitor;
struct Monitor{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    semaphore_t mutex;
    semaphore_t items;
    semaphore_t spaces;
};

void monitor_init(Monitor *m);

void monitor_produce(Monitor *m, int product);

int monitor_consume(Monitor *m);
