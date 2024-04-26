#ifndef SYS_CALL_H
#define SYS_CALL_H

#include "PCB.h"

//#define IDLE 0
//#define EXIT 1
// Add more operation codes as needed

context_t* sys_call(context_t* ctx);

#endif // SYS_CALL_H
