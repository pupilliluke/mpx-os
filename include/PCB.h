#ifndef PCB_H
#define PCB_H

typedef enum {
    //READY = 0
    READY,
    //BLOCKED = 1
    BLOCKED
    // Add other states here if needed
} pcb_state_E;

typedef enum {
    //NOT_SUSPENDED = 0
    NOT_SUSPENDED,
    //SUSPENDED = 1
    SUSPENDED
} pcb_state_D;

typedef enum{
    //SYSTEM = 0
    SYSTEM,
    //USER = 1
    USER
} pcb_class;

typedef struct pcb {
    char Name[9];
    pcb_class Class;
    int Priority;
    pcb_state_E State_E;
    pcb_state_D State_D;
    void* StackPtr;     //top
    char Stack[4096];   //bottom
    
    struct pcb* next;
} pcb;

//R3
   typedef struct context
{
    //Segments
    int ds;
    int es;
    int fs;
    int gs;
    int ss;
    //registers
    int esp;
    int eax;
    int ebx;
    int ecx;
    int edx;
    int esi;
    int edi;
    int ebp; 
    //flags
    int eip;
    int cs;
    int eflags;

} context_t;



extern pcb* current_pcb;

extern context_t* initial_ctx;

pcb* pcb_allocate(void);

int pcb_free(pcb*);

pcb* pcb_setup(const char*, int, int);

pcb* pcb_find(const char* name);

void pcb_insert(pcb* new_pcb);

int pcb_remove(pcb* target_pcb);

#endif
