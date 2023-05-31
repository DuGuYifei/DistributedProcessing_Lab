#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>

#define SEM_MSG_TYPE 1L

typedef struct binary_semaphore{
    int semaphore_id;
} binary_semaphore_t;


int binary_semaphore_init(binary_semaphore_t* sem, int init_state);
int binary_semaphore_wait(binary_semaphore_t* sem);
int binary_semaphore_post(binary_semaphore_t* sem);
int binary_semaphore_destroy(binary_semaphore_t* sem);