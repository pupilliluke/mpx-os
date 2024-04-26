#include "../include/stdlib.h"
#include "../include/memory.h"
#include "../include/queue.h"
#include <alarm.h>

queue* readyQueue;
queue* blockedQueue;

queue* create_queue(void) {
    queue* queuePtr = (queue*)sys_alloc_mem(sizeof(queue));
    if (queuePtr == NULL) {
        return NULL;
    }
    queuePtr->frontPtr = NULL;
    queuePtr->rearPtr = NULL;
    queuePtr->count = 0;
    return queuePtr;
}

int enqueue(queue* queuePtr, pcb* data) {
    if (queuePtr == NULL) {
        return 0;
    }
    node* newPtr = (node*)sys_alloc_mem(sizeof(node));
    if (newPtr == NULL) {
        return 0;
    }
    newPtr->data = data; // Assign the pointer directly
    newPtr->nextPtr = NULL;
    if (queuePtr->rearPtr != NULL) {
        queuePtr->rearPtr->nextPtr = newPtr;
        queuePtr->rearPtr = newPtr;
        queuePtr->count += 1;
    } else {
        queuePtr->frontPtr = newPtr;
        queuePtr->rearPtr = newPtr;
        queuePtr->count += 1;
    }
    return 1;
}

pcb* dequeue(queue* queuePtr) {
    if (is_empty(queuePtr)) {
        return NULL;
    }
    
    pcb* data = queuePtr->frontPtr->data;
    node* tempPtr = queuePtr->frontPtr;
    queuePtr->frontPtr = tempPtr->nextPtr;
    if (queuePtr->frontPtr == NULL) {
        queuePtr->rearPtr = NULL;
    }
    sys_free_mem(tempPtr);
    queuePtr->count =queuePtr->count - 1;
    return data;
}

int is_empty(queue* queuePtr) {
    if (queuePtr == NULL) {
        return 1;
    }
    return ((queuePtr->frontPtr == NULL) && (queuePtr->rearPtr == NULL));
}

