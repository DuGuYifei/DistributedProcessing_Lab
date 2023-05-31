#include "monitor.h"
#include "stdio.h"

void monitor_init(Monitor* monitor) {
    binary_semaphore_init(&monitor->mutex, 1);
    binary_semaphore_init(&monitor->condition, 0);
    monitor->waiting = 0;
}

void monitor_enter(Monitor* monitor) {
    binary_semaphore_wait(&monitor->mutex);
}

void monitor_exit(Monitor* monitor) {
    binary_semaphore_post(&monitor->mutex);
}

void monitor_wait(Monitor* monitor) {
    monitor->waiting++;
    binary_semaphore_post(&monitor->mutex);
    binary_semaphore_wait(&monitor->condition);
    monitor->waiting--;
}

void monitor_notify(Monitor* monitor) {
    if (monitor->waiting > 0) {
        binary_semaphore_post(&monitor->condition);
    }
}

void monitor_notify_out_monitor(Monitor* monitor) {
    if (monitor->waiting > 0) {
        binary_semaphore_wait(&monitor->mutex);
        binary_semaphore_post(&monitor->condition);
        binary_semaphore_post(&monitor->mutex);
    }
}
