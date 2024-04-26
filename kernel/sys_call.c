#include <PCB.h>
#include <queue.h>
#include <sys_req.h>
#include <mpx/serial.h>
#include <R6.h>
#include<memory.h>
// #define NULL (void*)0x000000 //works without it
extern pcb* current_pcb;
extern context_t* initial_ctx;
extern dcb* DCB;
void ioScheduler(status op, device dev, char* buf, int size, pcb* proc);

context_t* sys_call(context_t* ctx) {
    pcb* temp = NULL;

    //check for completed io
    //don't want to end up in situation where everything is blocked

    switch (ctx->eax) {
        case IDLE: {

            if (initial_ctx == NULL) {
                initial_ctx = ctx;
            }

            if (current_pcb != NULL)
            {   
                temp = current_pcb;
                current_pcb->StackPtr = (void*)ctx;
                current_pcb->State_E = READY;

            }

            if (readyQueue->count >= 1) {
                pcb* next_process = dequeue(readyQueue);
                current_pcb = next_process;
                ctx = (context_t*)current_pcb->StackPtr;

            }
                
            else {
                ctx = initial_ctx;
                initial_ctx = NULL;
            }
            if(temp != NULL){
                pcb_insert(temp);
            }
            ctx->eax = 0; // Return value for IDLE operation
        }
        break;

        case EXIT: {
            if (!is_empty(readyQueue)) {
                pcb* next_process = dequeue(readyQueue);
                pcb_free(current_pcb);
                current_pcb = next_process;
                ctx = (context_t*)current_pcb->StackPtr;
            } else {
                ctx = initial_ctx;
                initial_ctx = NULL;
            }
            ctx->eax = 0; // Return value for EXIT operation
        }
        break;

/*The device will be in EBX, buffer in ECX, and buffer size in EDX
    • Check to see whether the requested device is currently in use
    • If not, you can directly call the appropriate driver function (i.e.
        serial read() or serial write())
    • Otherwise, the request must be scheduled by your I/O scheduler (see below)*/

    /*
When scheduling I/O operations, process must be moved to BLOCKED
    state
    • Dispatch a new process as though the requested operation was IDLE
    • Each byte transferred will generate an interrupt
    • Device driver ISR will be invoked
    • If the operation is complete, set the Event Flag in the appropriate DCB
    • Each time sys call() is called, check for any available Event Flags
    • For any that are set, perform the required completion sequence*/
        
        case READ: {
            //get Device from EBX
            device dev = ctx->ebx;
            //get buffer from ECX
            char* buffer = (char*)ctx->ecx;
            //get buffer size from EDX 
            int buffer_size = ctx->edx;
            //Need fetch device DCB function
            dcb* DCB = getDcb();
            if(DCB->status == status_IDLE){
                serial_read(dev, buffer, buffer_size);
            }else{
                ioScheduler(status_READ, dev, buffer, buffer_size, current_pcb); 
            }
        }
        break;
        case WRITE: {
            // Write to the device
            //get Device from EBX
            device dev = ctx->ebx;
            //get buffer from ECX
            char* buffer = (char*)ctx->ecx;
            //get buffer size from EDX 
            int buffer_size = ctx->edx;
            //Need fetch device DCB function
            dcb* DCB = getDcb();
            if(DCB->status == status_IDLE){
                serial_write(dev, buffer, buffer_size);
            }else{
                ioScheduler(status_WRITE, dev, buffer, buffer_size, current_pcb);
            } 
        }
        break;

        default:
            ctx->eax = -1; // Invalid operation code
            break;
    }

    return ctx;
}

void ioScheduler(status op, device dev, char* buf, int size, pcb* proc)
{
    dcb* new  = getDcb();
    IOQueue* IOQ = getIOQ();
    if(op == READ || op == WRITE)
    {
        if(dev == COM1 && new != NULL)
        {
            if(buf != NULL)
            {
                if(size > 0)
                {
                    if(new->state == OPEN)
                    {
                        //begin processing request immediately
                        if(op == READ)
                        {
                            serial_read(dev, buf, size);
                        }    
                        else if(op == WRITE)
                        {
                            serial_write(dev, buf, size);
                        }
                    }
                    else{
                        iocb* newIocb = sys_alloc_mem(sizeof(iocb));
                        newIocb->buffer = buf;
                        newIocb->bufsize = size;
                        newIocb->device = dev;
                        newIocb->operation = op;
                        newIocb->requesting_process = proc;
                        newIocb->NextPtr = (struct iocb*)IOQ->head;
                        IOQ->head->PrevPtr = (struct iocb*)newIocb;
                        IOQ->head = newIocb;
                    }
                }
            }
        }
    }
    return;
}






