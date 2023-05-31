#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "monitor.h"

// int counter = 0;
// void* increment(void* arg) {
//     int i;
//     for (i = 0; i < 5; i++) {
//         monitor_enter(&monitor);

//         counter++;

//         printf("Counter value: %d\n", counter);

//         monitor_exit(&monitor);
//     }

//     pthread_exit(NULL);
// }

Monitor monitor;

void* thread1_function() {
    for (int i = 0; i < 5; i++) {
        printf("%d. Thread 1 plan to enter.\n", i);
        monitor_enter(&monitor);
        printf("%d. Thread 1 entered the monitor and waiting.\n", i);
        monitor_wait(&monitor);
        printf("%d. Thread 1 resumed execution.\n", i);
        monitor_exit(&monitor);
    }

    return NULL;
}

void* thread2_function() {
    for (int i = 0; i < 5; i++) {
        monitor_enter(&monitor);
        printf("Thread 2 entered the monitor and notifying.\n");
        monitor_notify(&monitor);
        printf("Thread 2 notified.\n");
        monitor_exit(&monitor);
        sleep(1);
    }

    return NULL;
}


int main() {
    monitor_init(&monitor);

    pthread_t thread1, thread2;
    //pthread_create(&thread1, NULL, increment, NULL);
    //pthread_create(&thread2, NULL, increment, NULL);

    pthread_create(&thread1, NULL, thread1_function, NULL);
    sleep(1);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
