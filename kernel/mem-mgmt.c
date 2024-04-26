#include <mem-mgmt.h>
#include <memory.h>
#include <mpx/vm.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>

void* heapPtr = NULL;
mem_list* free_list;
mem_list* alloc_list;
mem_list* alloc_list;


void initialize_heap(size_t size) {
    // heapPtr = kmalloc(size, 0, NULL);
    heapPtr = sys_alloc_mem(size);

    mcb* new_mcb = (mcb*)heapPtr;   //new mcb points to the start of the heap
                                //possibly change below
    new_mcb->Start_addr = (int)heapPtr + sizeof(mcb);   //returning the address at the end of the first mcb
    new_mcb->Size = size - sizeof(mcb);                 //49,984 
    new_mcb->NextPtr = NULL;                            //no nextPtr yet             
    // new_mcb->PrevPtr = NULL;                         //don't need prevPtr
    new_mcb->flag = 0;                                  //flag is set to free(0)
    
    // mcb* free_head = new_mcb;
    // mcb* alloc_head = NULL;

    // free_list->tailPtr->NextPtr = free_list->headPtr;  // circular list(to allow compaction from tail to head)
    alloc_list->headPtr = new_mcb;                      //set head
    alloc_list->tailPtr = new_mcb;                    //set tail                
    alloc_list->count++;                            //increment count               
}

void* allocate_memory(size_t size) {
    // mcb* temp_mcb = alloc_list->headPtr;
        mcb* temp_mcb = (mcb*)sys_alloc_mem(sizeof(mcb));

    int blocksCounted = 0;
    // int remaining_size = 50000 - (int)sizeof(mcb);
    //need iterate heap list until find free block big enough (First Fit)

    if (temp_mcb == NULL) {
        return NULL;
    }

    // Iterate through the heap list to find a block big enough
    while(temp_mcb != NULL && blocksCounted <= alloc_list->count)
    {

        //find free block 
        if( temp_mcb->flag == 0 ){
            if( temp_mcb->Size >= size + sizeof(mcb) ){     //is current free block big enough for request?

                //TARGET FOUND
                //create new mcb for allocated block
                mcb* allocated_mcb = (mcb*)temp_mcb->Start_addr;        //new mcb points to same address as temp_mcb

//SETTERS&UPDATES
                allocated_mcb->Start_addr = temp_mcb->Start_addr;       //set start address of allocated block
                allocated_mcb->Size = size;                             //set size of allocated block
                allocated_mcb->flag = 1;                                //set flag to allocated
                
                //NEXT PTR
                if( temp_mcb->NextPtr != NULL ){

// IF GAP BETWEEN CURRENT BLOCK AND NEXT BLOCK                          //+ 1????????
                    if( !((int)(allocated_mcb->Start_addr) + (int)size + (int)sizeof(mcb) + 1 == (int)(temp_mcb->NextPtr->Start_addr))){
                        //determine space between block and nextPtr
                        int gapSpace = temp_mcb->NextPtr->Start_addr - (allocated_mcb->Start_addr + size + sizeof(mcb));
                        //create new mcb for dead space
                        mcb* gap_space_mcb = (mcb*)(allocated_mcb->Start_addr + size + sizeof(mcb));  //new mcb points to end of allocated block
                        gap_space_mcb->Start_addr = allocated_mcb->Start_addr + size + sizeof(mcb);    //set start address of dead space mcb
                        gap_space_mcb->Size = gapSpace - sizeof(mcb) - 1;                             // set size of dead space mcb
                        gap_space_mcb->flag = 0;                                                       //set flag to free
                        gap_space_mcb->NextPtr = (mcb*)temp_mcb->NextPtr;                                     //set nextPtr of dead space mcb to nextPtr of free block
                        gap_space_mcb->PrevPtr = (mcb*)allocated_mcb;                                        //set prevPtr of dead space mcb to allocated block
                        temp_mcb->NextPtr->PrevPtr = (mcb*)gap_space_mcb;                                    //set prevPtr of nextPtr of free block to dead space mcb
                        allocated_mcb->NextPtr = (mcb*)gap_space_mcb;                                        //set nextPtr of allocated block to dead space mcb
                    }

                    allocated_mcb->NextPtr = (mcb*)(temp_mcb->NextPtr);          //set nextPtr of allocated block to nextPtr of free block
                    temp_mcb->NextPtr->PrevPtr = (mcb*)allocated_mcb;          //set prevPtr of nextPtr of free block to allocated block

                }//NEXT PTR WAS NULL
                else{
                    //need to create an mcb next open memory(free block)

                    mcb* end_mcb = (mcb*)temp_mcb->Start_addr + size + sizeof(mcb);  //new mcb points to end of heap
                    end_mcb->Start_addr = temp_mcb->Start_addr + size + sizeof(mcb);  //set start address of end mcb
                    end_mcb->Size = temp_mcb->Size - size - sizeof(mcb);              //set size of end mcb
                    allocated_mcb->NextPtr = NULL;                      //set nextPtr of allocated block to NULL
                }

                //FIRST ALLOCATION...HEAD PTR
                if( blocksCounted == 0) {
                    alloc_list->headPtr = allocated_mcb;                //set head to allocated block
                }
                //ELSE SET PREV PTR
                else{
                    temp_mcb->PrevPtr->NextPtr = allocated_mcb;         //set nextPtr of prevPtr of free block to allocated block
                    allocated_mcb->PrevPtr = temp_mcb->PrevPtr;          //set prevPtr of allocated block to prevPtr of free block
                }

                //NEXT AND PREV IS SET, HEAD IS SET, 



            }
        }
    }
    if(blocksCounted > alloc_list->count){
        sys_req(WRITE, COM1, "No free block big enough for the allocation request\n", 53);
        return NULL;
    }


//OLD CODE
    while (temp_mcb != NULL && blocksCounted < (int)free_list->count){
        if (temp_mcb->Size >= size) {
            size_t remaining_size = temp_mcb->Size - size;
            mcb* allocated_mcb = (mcb*)(temp_mcb->Start_addr);
            allocated_mcb->Size = size + sizeof(mcb);
            allocated_mcb->Start_addr = temp_mcb->Start_addr;
            allocated_mcb->PrevPtr = NULL;
            allocated_mcb->NextPtr = NULL;

            temp_mcb->Size = remaining_size;
            temp_mcb->Start_addr += (size + sizeof(mcb));

            if (alloc_list != NULL) {
                allocated_mcb->NextPtr = alloc_list->headPtr;
                if (alloc_list->headPtr != NULL) {
                    alloc_list->headPtr->PrevPtr = allocated_mcb;
                } else {
                    alloc_list->tailPtr = allocated_mcb;
                }
                alloc_list->headPtr = allocated_mcb;
                alloc_list->count++;
            }
            if (allocated_mcb->NextPtr != NULL) {
                allocated_mcb->NextPtr->PrevPtr = allocated_mcb;
            }
            return (void*)allocated_mcb->Start_addr;
        }
        blocksCounted++;
        temp_mcb = temp_mcb->NextPtr;
    }
    sys_req(WRITE, COM1, "No free block big enough for the allocation request\n", 53);
    return NULL;
}

int free_memory(void* memptr) {
    mcb* a_temp_mcb = alloc_list->headPtr;
    mcb* prev_a_temp_mcb = NULL;
    mcb* t_temp_mcb = (mcb*)sys_alloc_mem(sizeof(mcb));
    t_temp_mcb->NextPtr = NULL;
    t_temp_mcb->PrevPtr = NULL;
    t_temp_mcb->Start_addr = 0;
    t_temp_mcb->Size = 0;

    while (a_temp_mcb != NULL) {
        if (a_temp_mcb->Start_addr == (int)memptr) {
            t_temp_mcb->Start_addr = a_temp_mcb->Start_addr;
            t_temp_mcb->Size = a_temp_mcb->Size;

            // Remove from allocated list
            if (prev_a_temp_mcb == NULL) {  // First element
                alloc_list->headPtr = a_temp_mcb->NextPtr;
            } else {
                prev_a_temp_mcb->NextPtr = a_temp_mcb->NextPtr;
            }
            if (a_temp_mcb->NextPtr == NULL) {  // Last element
                alloc_list->tailPtr = prev_a_temp_mcb;
            }

            alloc_list->count--;
            break;
        }
        prev_a_temp_mcb = a_temp_mcb;
        a_temp_mcb = a_temp_mcb->NextPtr;
    }

    if (t_temp_mcb->Start_addr == 0 && t_temp_mcb->Size == 0) {  // Block not found in allocated list
        //sys_free_mem(t_temp_mcb);  // Free the temporary block
        return 1;
    }

    // Add to free list
    t_temp_mcb->NextPtr = free_list->headPtr;
    if (free_list->headPtr != NULL) {
        free_list->headPtr->PrevPtr = t_temp_mcb;
    }
    free_list->headPtr = t_temp_mcb;
    if (free_list->tailPtr == NULL || t_temp_mcb->NextPtr == NULL) {
        free_list->tailPtr = t_temp_mcb;  // Update tail pointer
    }
    free_list->count++;

    // Combine adjacent free blocks
    combine_free_blocks();

    return 0;
}

int check_valid(int memptr) {
    mcb* a_temp_mcb = alloc_list->headPtr;

    while (a_temp_mcb != NULL) {
        if (memptr == a_temp_mcb->Start_addr) {
            return 1;
        }
        a_temp_mcb = a_temp_mcb->NextPtr;
    }
    return 0;
}

void combine_free_blocks(void) {
    if (free_list == NULL || free_list->headPtr == NULL) {
        return;
    }

    
    int nodesChecked = 0;
    // Iterates from head to tail to compact the free list
    mcb* current = free_list->headPtr;
    //current != NULL && current->NextPtr != NULL && 
    int iterate = 0;
    while (nodesChecked < (int)free_list->count + 1) {
        iterate = 0;
        mcb* next = current->NextPtr;
        mcb* back = next->NextPtr;

        for(int i = 0; i < free_list->count; i++){
        if ((int)current->Start_addr + (int)current->Size == (int)next->Start_addr) {

            current->Size += next->Size - sizeof(mcb);
            if(next != next->NextPtr)
            {
            current->NextPtr = next->NextPtr;
            next->NextPtr->PrevPtr = current;
            }
            else{
                while(back->PrevPtr != current)
                {
                    back = back->PrevPtr;
                }
                current->NextPtr = back;
            }
            if(next == free_list->headPtr)
            {
                free_list->headPtr = current;
            }
            // Checks nextPtr has a node
            if (next->NextPtr != current && next->NextPtr != NULL) {
                next->NextPtr->PrevPtr = current;   // Set the 'next next' node prevPtr to current
            } else {
                free_list->tailPtr = current;  // Update tail pointer if next was the last element
            }

            //sys_free_mem(next);
            free_list->count--;
            // nodesChecked = 0;
            // char* nodesChecked_str = " ";
            // itoa(nodesChecked, nodesChecked_str);
            // sys_req(WRITE, COM1, nodesChecked_str, strlen(nodesChecked_str));
            // sys_req(WRITE, COM1, "\n", 2);
            current = free_list->headPtr;
            iterate = 1;
            i--;
            next = next->PrevPtr;
            if(free_list->count == 2)
            {
                while(current->PrevPtr != NULL)
                {
                    current = current->PrevPtr;
                }
            }
        }
        if((int)next->Start_addr + (int)next->Size == (int)current->Start_addr)
        {
            next->Size += current->Size;
            current->Start_addr = next->Start_addr;
            current->Size = next->Size - sizeof(mcb);
            current->NextPtr = next->NextPtr;
             // Checks nextPtr has a node
            if (next->NextPtr != current) {
                next->NextPtr->PrevPtr = current;   // Set the 'next next' node prevPtr to current
            } else {
                free_list->tailPtr = current;  // Update tail pointer if next was the last element
            }

            //sys_free_mem(next);
            free_list->count--;
            // nodesChecked = 0;
            // char* nodesChecked_str = " ";
            // itoa(nodesChecked, nodesChecked_str);
            // sys_req(WRITE, COM1, nodesChecked_str, strlen(nodesChecked_str));
            // sys_req(WRITE, COM1, "\n", 2); 
            current = free_list->headPtr;
            i--;
            next = next->PrevPtr;
            iterate = 1;
        }
        
        if(next != next->NextPtr){
        next = next->NextPtr;
        }
        else{
            while(next->PrevPtr != NULL)
            {
                next = next->PrevPtr;
            }
        }
        }
        if(iterate == 0){
            current = next;
            nodesChecked += 1; 
        }
        
    }
}
