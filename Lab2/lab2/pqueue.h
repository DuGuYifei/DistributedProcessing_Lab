typedef struct pqueue pqueue;
typedef struct pqueueNode pqueueNode;

struct pqueueNode
{
	void *data;
	pqueueNode *next;
	pqueueNode *prev;
};

struct pqueue
{
	pqueueNode *head;
	int (*cmp)(void *, void *);
	void (*print_data)(void *);
};

pqueue *init(int (*cmp)(void *, void *), void (*print_data)(void *));

void free_list(pqueue **list);

void qinsert(pqueue **list, void *data);

void qremove(pqueue **list, void *data);

void qlist(pqueue *list);
