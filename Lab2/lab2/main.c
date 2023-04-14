#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pqueue.h"

typedef struct process process;
struct process
{
	int k;
	char name[40];
};

int cmp(void *a, void *b)
{
	process *x = a;
	process *y = b;
	int intResult = x->k - y->k;
	return intResult ? intResult : strcmp(x->name, y->name);
}

void print_process(void *data)
{
	process *pr = (process *)data;
	printf("%d %s", pr->k, pr->name);
}

int main(int argc, char **argv)
{
	pqueue *queue = init(cmp, print_process);

	process *p1, *p2, *p3;
	p1 = (process *)malloc(sizeof(process));
	strcpy(p1->name, "Firefox");
	p1->k = 2;
	p2 = (process *)malloc(sizeof(process));
	strcpy(p2->name, "Geany IDE");
	p2->k = 5;
	p3 = (process *)malloc(sizeof(process));
	strcpy(p3->name, "Important Calculations");
	p3->k = 11;

	qinsert(&queue, p1);
	qinsert(&queue, p2);
	qinsert(&queue, p3);
	qlist(queue);

	printf("\n");
	qremove(&queue, p1);
	qlist(queue);

	printf("\n");
	qinsert(&queue, p1);
	qlist(queue);

	printf("\n");
	qremove(&queue, p2);
	qlist(queue);

	printf("\n");
	qremove(&queue, p3);
	qlist(queue);

	free_list(&queue);

	// free(p1);
	// free(p2);
	// free(p3);

	return 0;
}
