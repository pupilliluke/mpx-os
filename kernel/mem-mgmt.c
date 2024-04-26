#include <mem-mgmt.h>
#include <memory.h>
#include <mpx/vm.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>

void* heapPtr = NULL;

mem_list* alloc_list;


void initialize_heap(size_t size) {
    heapPtr = sys_alloc_mem(size);

    mcb* new_mcb = (mcb*)heapPtr;

    new_mcb->Start_addr = (int)heapPtr + sizeof(mcb);
    new_mcb->Size = size - sizeof(mcb);
    new_mcb->NextPtr = NULL;
    new_mcb->PrevPtr = NULL;
    new_mcb->flag = 0;


    

    alloc_list = (mem_list*)sys_alloc_mem(sizeof(mem_list));

    alloc_list->headPtr = new_mcb;
    alloc_list->tailPtr = new_mcb;
    alloc_list->count = 1;

   
}

void* allocate_memory(size_t size) {
    mcb* temp_head = alloc_list->headPtr;
    mcb* temp_mcb = temp_head;
    alloc_list->headPtr = temp_mcb;
    if (temp_mcb == NULL) {
        return NULL;
    }
   
    while (temp_mcb != NULL){
        if (temp_mcb->Size >= size && temp_mcb->flag == 0) {
            size_t remaining_size = temp_mcb->Size - size;


            mcb* allocated_mcb = (mcb*)(size + (int) temp_mcb->Start_addr);
            allocated_mcb->Size = size + sizeof(mcb);
            allocated_mcb->Start_addr = temp_mcb->Start_addr;
            allocated_mcb->PrevPtr = NULL;
            allocated_mcb->NextPtr = NULL;
            allocated_mcb->flag = 1;

            temp_mcb->Size = remaining_size;
            temp_mcb->Start_addr += (size + sizeof(mcb));
            

            if (alloc_list != NULL) {
                allocated_mcb->NextPtr = temp_mcb;
                if (temp_mcb != NULL) {
                    allocated_mcb->PrevPtr = temp_mcb->PrevPtr;
                    temp_mcb->PrevPtr = allocated_mcb;
                } else {
                    alloc_list->tailPtr = allocated_mcb;
                }
                if(temp_mcb == alloc_list->headPtr)
                {
                alloc_list->headPtr = allocated_mcb;
                }
                alloc_list->count++;
            }
            
            return (void*)allocated_mcb->Start_addr;
        }
        
        temp_mcb = temp_mcb->NextPtr;
    }
    sys_req(WRITE, COM1, "No free block big enough for the allocation request\n", 53);
    return NULL;
}

int free_memory(void* memptr) {

    //PROPOSED CHANGE: START AT TAILPTR AND ITERATE UNTIL NULL
    mcb* a_temp_mcb = alloc_list->tailPtr;
    mcb* t_temp_mcb = (mcb*)sys_alloc_mem(sizeof(mcb));
    t_temp_mcb->NextPtr = NULL;
    t_temp_mcb->PrevPtr = NULL;
    t_temp_mcb->Start_addr = 0;
    t_temp_mcb->Size = 0;
    //int done = 0;

    while (a_temp_mcb != NULL) {
        int again = 0;
        if (a_temp_mcb->Start_addr == (int)memptr) {    
            a_temp_mcb->flag = 0;
            
                
            if((a_temp_mcb != alloc_list->headPtr) && (a_temp_mcb->NextPtr != NULL) && a_temp_mcb->NextPtr->flag == 0)
            {
                a_temp_mcb->Size = a_temp_mcb->Size + a_temp_mcb->NextPtr->Size - sizeof(mcb);
                
                a_temp_mcb->NextPtr = a_temp_mcb->NextPtr->NextPtr;
                
                if(a_temp_mcb->NextPtr != NULL){
                a_temp_mcb->NextPtr->PrevPtr = a_temp_mcb;
                }
                alloc_list->count--;
                
            }
            
            if((a_temp_mcb->PrevPtr != NULL) && a_temp_mcb->PrevPtr->flag == 0)
                {
                    a_temp_mcb->PrevPtr->Size = a_temp_mcb->PrevPtr->Size + a_temp_mcb->Size - sizeof(mcb);
                    a_temp_mcb->PrevPtr->Start_addr = a_temp_mcb->Start_addr;
                    if(a_temp_mcb->NextPtr != NULL)
                    {
                        a_temp_mcb->NextPtr->PrevPtr = a_temp_mcb->PrevPtr;
                    }
                    a_temp_mcb->PrevPtr->NextPtr = a_temp_mcb->NextPtr;
                    alloc_list->count--;
                }
                else if(a_temp_mcb == alloc_list->headPtr)
                {
                    while(again == 0)
                    {
                        if(alloc_list->headPtr->flag == 0 && alloc_list->count > 1)
                        {
                            alloc_list->tailPtr->Start_addr = alloc_list->headPtr->Start_addr;
                            alloc_list->tailPtr->Size = alloc_list->tailPtr->Size + alloc_list->headPtr->Size - sizeof(mcb);
                            alloc_list->headPtr = alloc_list->headPtr->NextPtr;
                            alloc_list->headPtr->PrevPtr = NULL;
                            alloc_list->count--;
                        }
                        else{
                            again = 1;
                        }

                    }
                }
                    
        }
            
            a_temp_mcb = a_temp_mcb->PrevPtr;
            
    }
        //a_temp has traversed the list, failed to find the target memory block
        return 1;   //failed to free memory
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

// void combine_free_blocks(void) {
//     if (free_list == NULL || free_list->headPtr == NULL) {
//         return;
//     }

    
//     int nodesChecked = 0;
//     // Iterates from head to tail to compact the free list
//     mcb* current = free_list->headPtr;
//     //current != NULL && current->NextPtr != NULL && 
//     int iterate = 0;
//     while (nodesChecked < (int)free_list->count + 1) {
//         iterate = 0;
//         mcb* next = current->NextPtr;
//         mcb* back = next->NextPtr;

//         for(int i = 0; i < free_list->count; i++){
//         if ((int)current->Start_addr + (int)current->Size == (int)next->Start_addr) {

//             current->Size += next->Size - sizeof(mcb);
//             if(next != next->NextPtr)
//             {
//             current->NextPtr = next->NextPtr;
//             next->NextPtr->PrevPtr = current;
//             }
//             else{
//                 while(back->PrevPtr != current)
//                 {
//                     back = back->PrevPtr;
//                 }
//                 current->NextPtr = back;
//             }
//             if(next == free_list->headPtr)
//             {
//                 free_list->headPtr = current;
//             }
//             // Checks nextPtr has a node
//             if (next->NextPtr != current && next->NextPtr != NULL) {
//                 next->NextPtr->PrevPtr = current;   // Set the 'next next' node prevPtr to current
//             } else {
//                 free_list->tailPtr = current;  // Update tail pointer if next was the last element
//             }

//             //sys_free_mem(next);
//             free_list->count--;
//             // nodesChecked = 0;
//             // char* nodesChecked_str = " ";
//             // itoa(nodesChecked, nodesChecked_str);
//             // sys_req(WRITE, COM1, nodesChecked_str, strlen(nodesChecked_str));
//             // sys_req(WRITE, COM1, "\n", 2);
//             current = free_list->headPtr;
//             iterate = 1;
//             i--;
//             next = next->PrevPtr;
//             if(free_list->count == 2)
//             {
//                 while(current->PrevPtr != NULL)
//                 {
//                     current = current->PrevPtr;
//                 }
//             }
//         }
//         if((int)next->Start_addr + (int)next->Size == (int)current->Start_addr)
//         {
//             next->Size += current->Size;
//             current->Start_addr = next->Start_addr;
//             current->Size = next->Size - sizeof(mcb);
//             current->NextPtr = next->NextPtr;
//              // Checks nextPtr has a node
//             if (next->NextPtr != current) {
//                 next->NextPtr->PrevPtr = current;   // Set the 'next next' node prevPtr to current
//             } else {
//                 free_list->tailPtr = current;  // Update tail pointer if next was the last element
//             }

//             //sys_free_mem(next);
//             free_list->count--;
//             // nodesChecked = 0;
//             // char* nodesChecked_str = " ";
//             // itoa(nodesChecked, nodesChecked_str);
//             // sys_req(WRITE, COM1, nodesChecked_str, strlen(nodesChecked_str));
//             // sys_req(WRITE, COM1, "\n", 2); 
//             current = free_list->headPtr;
//             i--;
//             next = next->PrevPtr;
//             iterate = 1;
//         }
        
//         if(next != next->NextPtr){
//         next = next->NextPtr;
//         }
//         else{
//             while(next->PrevPtr != NULL)
//             {
//                 next = next->PrevPtr;
//             }
//         }
//         }
//         if(iterate == 0){
//             current = next;
//             nodesChecked += 1; 
//         }
        
//     }
// }
