#include <PCB.h>
#include <queue.h>
#include <sys_req.h>
#define NULL (void*)0x000000
extern pcb* current_pcb;
extern context_t* initial_ctx;

context_t* sys_call(context_t* ctx) {
    pcb* temp = NULL;

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

        default:
            ctx->eax = -1; // Invalid operation code
            break;
    }

    return ctx;
}



