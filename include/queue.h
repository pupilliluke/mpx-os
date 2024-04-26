#ifndef QUEUE_H
#define QUEUE_H
#include <PCB.h>

typedef struct queue
{
	struct queue_node* frontPtr;
	struct queue_node* rearPtr;
	int count;

}queue;

typedef struct queue_node {
    pcb* data; 
    struct queue_node* nextPtr;
} node;


int is_empty(queue* queuePtr);

pcb* dequeue(queue* queuePtr);

int enqueue(queue* queuePtr, pcb* data);

queue* create_queue(void);


extern queue* readyQueue;
extern queue* blockedQueue;


#endif /* QUEUE_H */

