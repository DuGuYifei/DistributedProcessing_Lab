#include "binary_semaphore.h"
#include <stdio.h>

typedef struct message{
	long mtype;
	char mtext;
} message;

// init_state 0/1
int binary_semaphore_init(binary_semaphore_t* sem, int init_state) {
    int msqid = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
    if (msqid == -1)
        return errno;
	
	// get status and set the max size of the queue to 1
    sem->semaphore_id = msqid;
	struct msqid_ds queue_info;
	if(msgctl(msqid, IPC_STAT, &queue_info)){
		perror("msgctl");
	}
	
	queue_info.msg_qbytes = sizeof(message);
	if(msgctl(msqid, IPC_SET, &queue_info) == -1){
		perror("msgctl");
	}

	if(init_state == 1){
		if(binary_semaphore_post(sem) == -1){
			printf("binary_semaphore_post\n");
		}
	}

    return 0;
}

int binary_semaphore_wait(binary_semaphore_t* sem) {
    message msg;

	// // check length of the queue
	// struct msqid_ds queue_info;
	// if(msgctl(sem->semaphore_id, IPC_STAT, &queue_info)){
	// 	perror("msgctl");
	// }
	// printf("queue_info.msg_qnum: %ld\n", queue_info.msg_qnum);

	// read, if no message, block!!!!!!!!
    // fd, buf, len, type, flag
    if (msgrcv(sem->semaphore_id, &msg, sizeof(message), SEM_MSG_TYPE, 0) == -1){
		printf("errno: %d have had msg\n", errno);
        return errno;
	}

    return 0;
}

int binary_semaphore_post(binary_semaphore_t* sem) {
    message msg;
	msg.mtext = 'v';
	msg.mtype = SEM_MSG_TYPE;

    // fd, buf, len, flag
    if (msgsnd(sem->semaphore_id, &msg, sizeof(message), IPC_NOWAIT) == -1){
		if (errno == EAGAIN) {
            //printf("Message queue is full.\n");
        } else {
            perror("msgsnd");
        }
	}

    return 0;
}

int binary_semaphore_destroy(binary_semaphore_t* sem) {
    if (msgctl(sem->semaphore_id, IPC_RMID, NULL) == -1)
        return errno;

    return 0;
}
