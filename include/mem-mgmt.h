#ifndef MEM_MGMT_H
#define MEM_MGMT_H
#include <stddef.h>

//Memory Control Block
typedef struct mcb{
    //The base address of the usable memory in the block (the first byte after the MCB)
    //Not where the MCB begins but where the chunk of memory allocated starts. MCB is placed aty the top of the memory block.
    int Start_addr;
    //The size of the block in bytes, not including the MCB
    size_t Size;
    //Next MCB in the list
    struct mcb* NextPtr;
    //Prev MCB in the list
    struct mcb* PrevPtr;

    int flag;

}mcb;

//memory list
typedef struct mem_list{
    mcb* headPtr;
    //possible not needed. Have not used it yet.
    mcb* tailPtr;
    int count;
}mem_list;


extern mem_list* free_list;
extern mem_list* alloc_list;

//helper method
void combine_free_blocks(void);

//initialize the heap
void initialize_heap(size_t size);
//allocate memory
void* allocate_memory(size_t size);
//  free memory
int free_memory(void *);

int check_valid(int);

#endif
