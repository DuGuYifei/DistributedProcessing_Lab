#include "binary_semaphore.h"

typedef struct Monitor{
    binary_semaphore_t mutex;
    binary_semaphore_t condition;
    int waiting;
} Monitor;

void monitor_init(Monitor* monitor);
void monitor_enter(Monitor* monitor);
void monitor_exit(Monitor* monitor);
void monitor_wait(Monitor* monitor);
void monitor_notify(Monitor* monitor);
void monitor_notify_out_monitor(Monitor* monitor);