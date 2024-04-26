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
    char Stack[1024];
    void* StackPtr;
    struct pcb* next;
} pcb;

pcb* pcb_allocate(void);

int pcb_free(pcb*);

pcb* pcb_setup(const char*, int, int);

pcb* pcb_find(const char* name);

void pcb_insert(pcb* new_pcb);

int pcb_remove(pcb* target_pcb);

//R3 
typedef struct context {

    unsigned int ESP; //Stack pointer (in the SS segment). // gets pushed last

    // Segment Registers
    unsigned int SS; //stack segment register
         //data segment registers
    unsigned int DS; // Data Segment
    unsigned int ES; // Extra Segment
    unsigned int FS; // F Segment
    unsigned int GS; // G Segment


    // General Purpose Registers
    unsigned int EAX; //EAX—Accumulator for operands and results data.
    unsigned int EBX; //Pointer to data in the DS segment.
    unsigned int ECX; //Counter for string and loop operations.
    unsigned int EDX; //I/O pointer.
    unsigned int ESI; //Pointer to data in the segment pointed to by the DS register; source pointer for string operations.
    unsigned int EDI; //Pointer to data (or destination) in the segment pointed to by the ES register; destination pointer for string operations.
    unsigned int EBP; //Pointer to data on the stack (in the SS segment).

        // Status and Control Register
    unsigned int EIP;    //  can also be called "program counter." It contains the offset in the current code segment for the next instruction to be executed.

    unsigned int CS; //Stack pointer (in the SS segment).

        // Status and Control Register
    unsigned int EFLAGS; //  contains a group of status flags, a control flag, and a group of system flags
//gets pushed first

}context;



#endif
