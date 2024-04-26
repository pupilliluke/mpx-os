#include "../include/stdlib.h"
#include "../include/memory.h"
#include "../include/PCB.h"
#include "../include/string.h"
#include "../include/queue.h"
#include "../include/sys_req.h"

#define MAX_PCBS 100 // Maximum number of PCBs, adjust as needed

//static pcb* pcb_list[MAX_PCBS]; // Array to hold PCB pointers
//static int pcb_count = 0;        // Variable to keep track of PCB count

queue* ready; 
queue* blocked; 
queue* suspended;


pcb* pcb_allocate(void){
    pcb* new_pcb = (pcb*)sys_alloc_mem(sizeof(pcb));

    //Check for failed allocation
    if(new_pcb == NULL){
        return NULL;
    }
    return new_pcb;
}
//returns -1 on error and 0 on success
int pcb_free(pcb* pcb){
    if(pcb == NULL){
        //return error code
        return -1;
    }
    sys_free_mem(pcb);
    //return success code
    return 0;
}

pcb* pcb_setup(const char* name, int class, int priority){
    pcb* new_pcb = pcb_allocate();
     if (name == NULL || (class != 0 && class != 1) || priority < 0 || priority > 9) {
        pcb_free(new_pcb);
        return NULL;
    }
    if(new_pcb == NULL){
        return NULL;
    }
    copy_string(new_pcb->Name, name, sizeof(new_pcb->Name) - 1);
    new_pcb->Name[sizeof(new_pcb->Name) - 1] = '\0';
    new_pcb->Class = class;
    new_pcb->Priority = priority;
    new_pcb->State_E = READY;
    new_pcb->State_D = NOT_SUSPENDED;
    new_pcb->StackPtr = &(new_pcb->Stack[sizeof(new_pcb->Stack) - 1]);
    memset(new_pcb->Stack, 0, sizeof(new_pcb->Stack));
    return new_pcb;
}

pcb* pcb_find(const char* name) {
            if (readyQueue != NULL){
                node* current = readyQueue->frontPtr;     
                while(current != NULL){
                    if (strcmp(current->data->Name, name) == 0) {
                        return current->data;
                    }
                    current = current->nextPtr;
                }
            }

            if (blockedQueue != NULL){
                node* current = blockedQueue->frontPtr;     
            
                while(current != NULL){
                    if (strcmp(current->data->Name, name) == 0) {
                        return current->data;
                    }
                    current = current->nextPtr;
                } 
            }
            return NULL; // PCB not found
        }
void pcb_insert(pcb* new_pcb) {
    node* newPtr = (node*)sys_alloc_mem(sizeof(node));
    if (newPtr == NULL) {
        sys_req(WRITE, COM1, "Here", 5);
        return;
    }
    newPtr->data = new_pcb; // Assign the pointer directly
    newPtr->nextPtr = NULL;

    int prio = new_pcb->Priority;

    // Assume ready Queue and blocked Queue are your queues for ready and blocked processes
    if (new_pcb->State_E == READY) {
        //If the ready queue does not exist yet, create the ready queue
        if(readyQueue == NULL){
            readyQueue = create_queue();
        }

            node* current = readyQueue->frontPtr;
            node* temp = NULL;
           // node* check;
            //int highest = -1;
            while(current != NULL) {
                if (current->data->Priority <= prio) {
                    //highest = prio;

                    //if (current->nextPtr != NULL) {
                       // check = current->nextPtr;
                        temp = current;

                    //}

                } else {
                    //highest = current->data->Priority;
                }
                current = current->nextPtr;
            }
                //node* temp2 = readyQueue->rearPtr;
                
               // node* insTemp = NULL;
            if(readyQueue->count > 1) {
                if(temp == NULL){
                    
                    node* frontTemp = readyQueue->frontPtr;
                    readyQueue->frontPtr = newPtr;
                    newPtr->nextPtr = frontTemp;
                    readyQueue->count++;
                }
               
                else{
                    node* tnode = temp->nextPtr;
                    temp->nextPtr = newPtr;
                    newPtr->nextPtr = tnode;
                    readyQueue->count++;
                }

            }
            
            else if(readyQueue->count == 1)
            {
                if(readyQueue->frontPtr->data->Priority < newPtr->data->Priority)
                {
                    enqueue(readyQueue, new_pcb);
                }
                else{
                    node* temp = readyQueue->frontPtr;
                    readyQueue->frontPtr = newPtr;
                    newPtr->nextPtr = temp;
                    readyQueue->count++;
                }
            }
            else{
                enqueue(readyQueue, new_pcb);
            }
    }
    else if (new_pcb->State_E == BLOCKED) {
        //If blocked queue does not exist yet, create blocked queue
        if(blockedQueue == NULL){
            blockedQueue = create_queue();
        }
        // If the process is blocked, add it to the blockedQueue
        enqueue(blockedQueue, new_pcb);
    }
}


int pcb_remove(pcb* target_pcb) {
    if (target_pcb == NULL) {
        return 0; // Invalid PCB
    }
    if(target_pcb->State_E == READY)
    {
    node* tempNode = readyQueue->frontPtr;
    while(tempNode != NULL)
    {
    if(readyQueue->count == 1){
    if(!strcmp(target_pcb->Name,tempNode->data->Name))
    {
        dequeue(readyQueue);
        return 1;
    }
    else{
        return 0;
    }
    }
    else if(readyQueue->count > 1)
    {
        if(!strcmp(tempNode->data->Name, target_pcb->Name))
        {
            dequeue(readyQueue);
            return 1;
        }
        else{
        while(tempNode->nextPtr != NULL)
        {
            if(!strcmp(tempNode->nextPtr->data->Name, target_pcb->Name))
            {
                if(tempNode->nextPtr->nextPtr != NULL){
                    tempNode->nextPtr = tempNode->nextPtr->nextPtr;
                    readyQueue->count-= 1;
                    return 1;
                }
                else{
                    tempNode->nextPtr = NULL;
                    readyQueue->count-= 1;
                    return 1;
                }
            }
            tempNode = tempNode->nextPtr;
        }
        return 0;
    }
    }
    }
    }
    else if(target_pcb->State_E == BLOCKED)
    {
    node* tempNode = blockedQueue->frontPtr;
    while(tempNode != NULL)
    {
    if(blockedQueue->count == 1){
    if(!strcmp(target_pcb->Name,tempNode->data->Name))
    {
        dequeue(blockedQueue);
        return 1;
    }
    else{
        return 0;
    }
    }
    else if(blockedQueue->count > 1)
    {
        if(!strcmp(tempNode->data->Name, target_pcb->Name))
        {
            dequeue(readyQueue);
            return 1;
        }
        else{
        while(tempNode->nextPtr != NULL)
        {
            if(!strcmp(tempNode->nextPtr->data->Name, target_pcb->Name))
            {
                if(tempNode->nextPtr->nextPtr != NULL){
                    tempNode->nextPtr = tempNode->nextPtr->nextPtr;
                    blockedQueue->count-= 1;
                    return 1;
                }
                else{
                    tempNode->nextPtr = NULL;
                    blockedQueue->count-= 1;
                    return 1;
                }
            }
            tempNode = tempNode->nextPtr;
        }
        return 0;
        }
    }
    }
    }
    return 0;
}

