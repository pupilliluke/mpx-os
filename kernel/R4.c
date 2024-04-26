#include <memory.h>
#include <string.h>
#include <PCB.h>
#include <sys_call.h>
#include <processes.h>
#include <com_hand.h>
#include <sys_req.h>
#include <queue.h>
#include <stdlib.h>
#include <alarm.h>
//com hand proccess
void comhand_setup(void){
    pcb* pcb = pcb_setup("ComHand", SYSTEM, 0);
    pcb->StackPtr = pcb->Stack + sizeof(pcb->Stack) - sizeof(context_t);
    context_t* ctx = (context_t*)pcb->StackPtr;
    //memset(ctx, 0, sizeof(context_t));

    //Segments
    ctx->cs = (unsigned int)0x08;
    ctx->ds = (unsigned int)0x10;   
    ctx->es = (unsigned int)0x10;   
    ctx->fs = (unsigned int)0x10;   
    ctx->gs = (unsigned int)0x10;   
    ctx->ss = (unsigned int)0x10; 

    //Registers
    ctx->esp = (unsigned int) pcb->StackPtr;
    ctx->eax = (unsigned int) 0x00; 
    ctx->ebx = (unsigned int) 0x00; 
    ctx->ecx = (unsigned int) 0x00; 
    ctx->edx = (unsigned int) 0x00;  
    ctx->esi = (unsigned int) 0x00; 
    ctx->edi = (unsigned int) 0x00; 
    ctx->ebp = (unsigned int)pcb->Stack;

    //Flags
    ctx->eip = (unsigned int) com_hand;  
    ctx->eflags =(unsigned int)0x0202;

    pcb_insert(pcb);
}

//IDLE proccess
void sys_idle_setup(void){
    pcb* pcb = pcb_setup("IDLE_Proc", SYSTEM, 9);
    pcb->StackPtr = pcb->Stack + sizeof(pcb->Stack) - sizeof(context_t);
    context_t* ctx = (context_t*)pcb->StackPtr;
    memset(ctx, 0, sizeof(context_t));
    //Segments
    ctx->cs = (unsigned int)0x08;
    ctx->ds = (unsigned int)0x10;   
    ctx->es = (unsigned int)0x10;   
    ctx->fs = (unsigned int)0x10;   
    ctx->gs = (unsigned int)0x10;   
    ctx->ss = (unsigned int)0x10; 

    //Registers
    ctx->esp = (unsigned int) pcb->StackPtr;
    ctx->eax = (unsigned int) 0x00; 
    ctx->ebx = (unsigned int) 0x00; 
    ctx->ecx = (unsigned int) 0x00; 
    ctx->edx = (unsigned int) 0x00; 
    ctx->esi = (unsigned int) 0x00; 
    ctx->edi = (unsigned int) 0x00;
    ctx->ebp = (unsigned int)pcb->Stack;

    //Flags
    ctx->eip = (unsigned int) sys_idle_process;  
    ctx->eflags =(unsigned int)0x0202;

    pcb_insert(pcb);
}

//Alarm process
void alarm_setup(void){

    pcb* pcb = pcb_setup("Alarm", USER, 2);
    pcb->StackPtr = pcb->Stack + sizeof(pcb->Stack) - sizeof(context_t);
    context_t* ctx = (context_t*)pcb->StackPtr;
    memset(ctx, 0, sizeof(context_t));
    //Segments
    ctx->cs = (unsigned int)0x08;
    ctx->ds = (unsigned int)0x10;   
    ctx->es = (unsigned int)0x10;   
    ctx->fs = (unsigned int)0x10;   
    ctx->gs = (unsigned int)0x10;   
    ctx->ss = (unsigned int)0x10; 

    //Registers
    ctx->esp = (unsigned int) pcb->StackPtr;
    ctx->eax = (unsigned int) 0x00; 
    ctx->ebx = (unsigned int) 0x00; 
    ctx->ecx = (unsigned int) 0x00; 
    ctx->edx = (unsigned int) 0x00; 
    ctx->esi = (unsigned int) 0x00; 
    ctx->edi = (unsigned int) 0x00;
    ctx->ebp = (unsigned int)pcb->Stack;

    //Flags
    ctx->eip = (unsigned int) check_alarm;  
    ctx->eflags =(unsigned int)0x0202;

    pcb_insert(pcb);
}


