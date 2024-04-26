#include <PCB.h>
#ifndef MPX_R6_H

#define MPX_R6_H


/**
 * R6.h
 * 
 * This file contains the data structures for R6
*/

typedef enum port_allocation_status{
    OPEN = 0,           //used to be available and busy
    CLOSED = 1,
}state;

typedef enum status_operation{
    status_READ = 0,
    status_WRITE = 1,
    status_IDLE = 2,
}status;

typedef enum event_flag{
    COMPLETE = 1,   
    INCOMPLETE = 0,
}event_flag;


typedef struct DeviceControlBlock{
    state state;
    status status;
    event_flag flag;    

    //for serial read interrupt if code IS NOT READ we fill ring buffer
    char ring_buffer[16]; //for saving the keystrokes the user types while in WRITE
    int ringin;  // next location in the ringbuffer to store a char
    int ringout; // next char to remove from ringbuffer
    int ringcount;  //num chars in ring buffer    

    char* input_buffer;     //serial READ ONLY
    int current_location;
    int buffer_size;


    int characters_transfered;      
    char* output_buffer;    //WRITE ONLY

    //where do we hold the queue of iocbs?

}dcb;
//sys_req call
//if opcode is read & device is available (status), then we call serial read
// params from sys req are passed to serial read
//serial read stores buffer dcb input buffer
//serial read replaces polling
//same idea as the filling the buffer in r1 polling ( for serial read ) 


typedef struct iocb{

    pcb* requesting_process;
    device device;
    status operation;
    char* buffer;

     //AKA I/O descriptor
    int bufsize; 
    
    struct iocb* NextPtr;
    struct iocb* PrevPtr;

}iocb;

/*
Holds queue of IOCB blocks
*/
typedef struct IOQueue{
    iocb* head;
    iocb* tail;
    int count;
}IOQueue;

//WILL NOT NEED LIST OF DCBs, only implementing 1 DCB (for COM1)



#endif
