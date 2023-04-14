#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"


pqueue* init(int (*cmp)(void *, void *), void (*print_data)(void *))
{
	pqueue *list = (pqueue*)malloc(sizeof(pqueue));
	list->head = NULL;
	list->cmp = cmp;
	list->print_data = print_data;
	return list;
}

void free_list(pqueue **list)
{
	pqueueNode *p = (*list)->head;
	while (p)
	{
		pqueueNode *next = p->next;
		free(p->data);
		free(p);
		p = next;
	}
	free(*list);
}

void qinsert(pqueue **list, void *data)
{
	pqueueNode *p = (pqueueNode *)malloc(sizeof(pqueueNode));
	p->data = data;
	p->prev = NULL;
	p->next = NULL;

	if (!(*list)->head)
	{
		(*list)->head = p;
	}
	else
	{
		pqueueNode *cur = (*list)->head;
		while (cur)
		{
			if ((*list)->cmp(cur->data, data) >= 0)
			{
				if (cur->prev)
				{
					cur->prev->next = p;
					p->next = cur;
					cur->prev = p;
				}
				else
				{
					(*list)->head = p;
					p->next = cur;
					cur->prev = p;
				}
				return;
			}
			if (!cur->next)
			{
				cur->next = p;
				p->prev = cur;
				return;
			}
			cur = cur->next;
		}
	}
}

void qremove(pqueue **list, void *data)
{
	pqueueNode *cur = (*list)->head;
	while (cur)
	{
		if (!(*list)->cmp(cur->data, data))
		{
			pqueueNode *prev = cur->prev;
			pqueueNode *next = cur->next;
			if (prev)
			{
				if (next)
				{
					prev->next = next;
					next->prev = prev;
				}
				else
				{
					prev->next = NULL;
				}
			}
			else
			{
				(*list)->head = next;
				if (next)
				{
					next->prev = NULL;
				}
			}
			free(cur);
			return;
		}
		cur = cur->next;
	}
}

void qlist(pqueue *list)
{
	pqueueNode *p;

	for (p = list->head; p != NULL; p = p->next)
	{
		list->print_data(p->data);
		printf("\n");
	}
}