#include <com_hand.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <string.h>
#include <time_date.h>
#include <stdlib.h>
#include <PCB.h>
#include <queue.h>
#include <sys_call.h>
#include <processes.h>
#include <stdarg.h>
#include <stddef.h>
#include <alarm.h>
#include <mem-mgmt.h>


void custom_itoa(int num, char* str);
void show_allocated_memory(void);
void show_free_memory(void);
void printHex(unsigned int num);
unsigned int reverseHex(const char* hexString);
void show_allocated_memory(void);


void* startAddress = NULL;

void trim_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
int first = 1;
void com_hand(void) {

    
    //declaring lines
/*     char logo1[100] = {"  ________  ___       _______   _______   ________  ________  ________      \n"};
    char logo2[100] = {" |\\   ____\\|\\  \\     |\\  ___ \\ |\\  ___ \\ |\\   __  \\|\\   __  \\|\\   ____\\     \n"};
    char logo3[100] = {" \\ \\  \\____\\ \\  \\    \\ \\   __/|\\ \\   __/|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|_    \n"};
    char logo4[100] = {"  \\ \\_____  \\ \\  \\    \\ \\  \\    \\ \\  \\    \\ \\   ____\\ \\  \\\\  \\ \\_____  \\   \n"};
    char logo5[100] = {"   \\|____|\\  \\ \\  \\____\\ \\  \\_|\\ \\ \\  \\_|\\ \\ \\  \\___|\\ \\  \\\\  \\|____|\\  \\  \n"};
    char logo6[100] = {"     ____\\_\\  \\ \\_______\\ \\_______\\ \\_______\\ \\__\\    \\ \\_______\\____\\_\\  \\ \n"};
    char logo7[100] = {"    |\\_________\\|_______|\\|_______|\\|_______|\\|__|     \\|_______|\\_________\\\n"};
    char logo8[100] = {"    \\|_________|                                                \\|_________|\n"};
*/
    char welcome[70] = {"Welcome to SleepOS, enter 'help' to read all available commands\n"};

    
    char newline[2] = {"\n"};
    char sos[5] = {">>"}; 
    
    
    
    
    //sys_req(WRITE, COM1, newline, 2);

   
 //write startup page

    // sys_req(WRITE,COM1,newline,2);        //newline
    // sys_req(WRITE,COM1,newline,2);    
    sys_req(WRITE,COM1,newline,2);

    /* sys_req(WRITE,COM1,logo1,100);          //logo
    sys_req(WRITE,COM1,logo2,100);
    sys_req(WRITE,COM1,logo3,100);
    sys_req(WRITE,COM1,logo4,100);
    sys_req(WRITE,COM1,logo5,100);
    sys_req(WRITE,COM1,logo6,100);
    sys_req(WRITE,COM1,logo7,100);
    sys_req(WRITE,COM1,logo8,100); */

    sys_req(WRITE,COM1,welcome,70);         //welcome message

    sys_req(WRITE, COM1, newline, 2);       //newline & sos
    sys_req(WRITE, COM1, newline, 2);

    //int init_heap_flag = 0;

    while (1){
        
        
        char buff[100] = {0};
        sys_req(WRITE,COM1, sos, 5);
        sys_req(READ, COM1, buff, 100);

        // //checks if we've already initialized heap
        // if( !init_heap_flag ){
        //     sys_req(WRITE,COM1, "Heap initialized at start address: ", 36);
        //     //initialize heap 1 time, to allow for allocate_mem(), needs to be in while loop
        //     initialize_heap(50000);
        //     sys_req(WRITE,COM1, newline, 2);
        //     init_heap_flag = 1;
        //     // memset(buff, 0, sizeof(buff));

        // }

        trim_newline(buff);  // Trim the newline character from the input


        if(!strcmp(buff, "help")|| !strcmp(buff, "Help"))
        {
           sys_req(WRITE,COM1, "Current Comands:\n", 18);
           sys_req(WRITE, COM1, "Help - List all implemented commands\n", 38);
           sys_req(WRITE, COM1, "Version - display verion\n", 26);
           sys_req(WRITE, COM1, "Get Time - gets current time\n", 30);
           sys_req(WRITE, COM1, "Set Time (Second:Hour:Minute) - sets a new current time\n", 57);
           sys_req(WRITE, COM1, "Get Date - gets the current date\n", 34);
           sys_req(WRITE, COM1, "Set Date (Day/Month/Year) - sets a new current date \n", 54);

           sys_req(WRITE, COM1, "\n", 2);

           //sys_req(WRITE, COM1, help9, 32);
           sys_req(WRITE, COM1, "Delete PCB - deletes a PCB\n", 28);
           sys_req(WRITE, COM1, "Block PCB - blocks a PCB\n", 26);
           sys_req(WRITE, COM1, "Unblock PCB - unblocks a PCB\n", 30);
           sys_req(WRITE, COM1, "Suspend PCB - suspends a PCB process\n", 38);
           sys_req(WRITE, COM1, "Resume PCB - resumes a suspended PCB process\n", 45);
           sys_req(WRITE, COM1, "Set PCB Priority - changes a process priority\n", 47);
           sys_req(WRITE, COM1, "Show PCB - shows all data assigned to the PCB\n", 47);
           sys_req(WRITE, COM1, "Show Ready - shows all processes in the ready queue\n", 53);
           sys_req(WRITE, COM1, "Show Blocked - shows all processes in the blocked queue\n", 57);
           sys_req(WRITE, COM1, "Show all -  shows all processes in all queue\n", 46);

           sys_req(WRITE, COM1, "\n", 2);


           sys_req(WRITE, COM1, "Load R3 - Loads the R3 test processes from <processes.h>\n", 58);
           sys_req(WRITE, COM1, "Yield - Causess command handler to yield to the CPU\n", 53);
           sys_req(WRITE, COM1, "Alarm - creates an alarm at a desired time\n", 44);

           sys_req(WRITE, COM1, "\n", 2);
           
           sys_req(WRITE, COM1, "Allocate Memory (AM) - allocates a given amount of memory in the heap\n", 71);
           sys_req(WRITE, COM1, "Free Memory (FM) - frees a given of block from the heap\n", 57);
           sys_req(WRITE, COM1, "Show Allocated Memory (SAM) - shows all allocated memory blocks in the heap\n", 77);
           sys_req(WRITE, COM1, "Show Free Memory (SFM) - shows all freed blocks in the heap\n", 61);
           sys_req(WRITE, COM1, "Show All Memory (SA) - shows all blocks in the heap, freed and allocated\n", 74);


           sys_req(WRITE, COM1, "\n", 2);
         
           
           sys_req(WRITE, COM1, "Shutdown - Shuts down the system\n", 34);

           
          


           
           for (int i = 0; i<100; i++)
           {
            buff[i] = 0;
           }
           //memset(buff, 0, sizeof(buff));
           
        }
        else if (!strcmp(buff, "shutdown") || !strcmp(buff, "Shutdown") || !strcmp(buff, "quit"))
        {
            //1 if input is invalid, 0 if valid
            int invalid = 1;
            //input buffer
            char confirm[4] = {0};
            //loop until valid input
            while(invalid){
                //confirm shutdown
                sys_req(WRITE, COM1, "Shutdown MPX? Yes(y) or No(n): ", 32);
                sys_req(READ, COM1, confirm, 4);
                //yes to shutdown
                if(!strcmp(confirm, "Yes") || !strcmp(confirm, "yes") || !strcmp(confirm, "Y") || !strcmp(confirm, "y")){
                    invalid = 0;
                    pcb_remove(pcb_find("Alarm"));
                     sys_req(EXIT);
                }
                //No to shutdown
                else if(!strcmp(confirm, "No") || !strcmp(confirm, "no") || !strcmp(confirm, "N") || !strcmp(confirm, "n")){
                    break;
                }
                //invalid selection
                else{
                    sys_req(WRITE, COM1, "Invalid selection. Please type y or n\n", 39);
                    invalid = 1;
                }
            }
        }
        //Version command
        else if (!strcmp(buff, "version") || !strcmp(buff, "Version"))
        {
            sys_req(WRITE, COM1, "Version R5\n",12);
        }
        //Get Time command
        else if (!strcmp(buff, "Get Time") || !strcmp(buff, "get time") || !strcmp(buff, "gettime") || !strcmp(buff, "GetTime")){
           getTime();
        }
        //Set Time command
        else if (!strcmp(buff, "Set Time") || !strcmp(buff, "set time") || !strcmp(buff, "settime") || !strcmp(buff, "SetTime")){
            int valid_hour = 0;
            int valid_min = 0;
            int valid_sec = 0;
            char hr[20] = {0};
            char min[3] = {0};
            char sec[3] = {0};
            int hour = 0;
            int second = 0;
            int minute = 0;
            sys_req(WRITE, COM1, "Please enter the hour (0-24): \n", 32);
            while(valid_hour == 0){

                sys_req(READ, COM1, hr, 3);


                //check if hr is all valid digits 0-9
                if ( isNum(hr)){

                    hour = atoi(hr);
                    if(hour < 0 || hour > 24)
                        {
                            sys_req(WRITE, COM1, "\nInvalid time. Please enter a number between 0 and 24: \n", 57);
                        } 
                    else
                    {
                        valid_hour = 1;
                    }

                trim_newline(hr);  // Trim the newline character from the input
                hour = atoi(hr);
                if(hour < 0 || hour > 24){
                sys_req(WRITE, COM1, "Invalid time. Please enter a number between 0 and 24: ", 54);
                
                } else{
                    valid_hour = 1;

                }
                }
                else //if isNum() returns false, write invalid char
                {
                    sys_req(WRITE, COM1, "\nInvalid character. Please enter a valid digit between 0 and 24: \n", 67);
                    //memset(buff, 0, sizeof(buff));
                }

            }
            sys_req(WRITE, COM1, "Please enter the minute (0-59): \n", 34);
            while(valid_min == 0){
                sys_req(READ, COM1, min, 3);
                trim_newline(min);  // Trim the newline character from the input
                minute = atoi(min);
                if(minute < 0 || minute > 59){
                sys_req(WRITE, COM1, "Invalid time. Please enter a number between 0 and 59: \n", 56);
                
                } else{
                    valid_min = 1;
                }
            }
             sys_req(WRITE, COM1, "Please enter the second (0-59): \n", 34);
            while(valid_sec == 0){
                sys_req(READ, COM1, sec, 3);
                trim_newline(sec);  // Trim the newline character from the input
                second = atoi(sec);
                if(second < 0 || second > 59){
                sys_req(WRITE, COM1, "Invalid time. Please enter a number between 0 and 59: \n", 56);
                
                } else{
                    valid_sec = 1;
                }
            }
            setTime(second, minute, hour);
            sys_req(WRITE, COM1, "The Time has been set.\n", 24);
        }

        //set date  
        else if (!strcmp(buff, "Set Date") || !strcmp(buff, "set date") || !strcmp(buff, "setdate") || !strcmp(buff, "SetDate")) {
            int valid_day = 0;
            int valid_month = 0;
            int valid_year = 0;
            char dy[3] = {0};
            char mn[3] = {0};
            char yr[3] = {0};
            int day = 0;
            int month = 0;
            int year = 0;

            sys_req(WRITE, COM1, "Please enter the year (0-24): ", 31);
            while (valid_year == 0) {
                sys_req(READ, COM1, yr, 3);
                trim_newline(yr);  // Trim the newline character from the input
                year = atoi(yr);
                if (year < 0 || year > 24) {
                    sys_req(WRITE, COM1, "Invalid year. Please enter a number between 0 and 24: ", 56);
                } else {
                    valid_year = 1;
                }
            }

            sys_req(WRITE, COM1, "Please enter the month (1-12): ", 32);
            while (valid_month == 0) {
                sys_req(READ, COM1, mn, 3);
                trim_newline(mn);  // Trim the newline character from the input
                month = atoi(mn);
                if (month < 1 || month > 12) {
                    sys_req(WRITE, COM1, "Invalid month. Please enter a number between 1 and 12: ", 57);
                } else {
                    valid_month = 1;
                }
            }

            int max_days;
            if (month == 4 || month == 6 || month == 9 || month == 11) {
                max_days = 30; // April, June, September, November have 30 days
            } else if (month == 2) {
                // Check for leap year
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                    max_days = 29; // February has 29 days in leap year
                } else {
                    max_days = 28; // February has 28 days in non-leap year
                }
            } else {
                max_days = 31; // other months have 31 days
            }

            char max_days_str[3];
            custom_itoa(max_days, max_days_str);

            sys_req(WRITE, COM1, "Please enter the day (1-", 25);
            sys_req(WRITE, COM1, max_days_str, 3); // Display the maximum days
            sys_req(WRITE, COM1, "): ", 4);


            while (valid_day == 0) {
                sys_req(READ, COM1, dy, 3);
                trim_newline(dy);  // Trim the newline character from the input
                day = atoi(dy);
                if (day < 1 || day > max_days) {
                    sys_req(WRITE, COM1, "Invalid day. Please enter a number between 1 and ", 50);
                    sys_req(WRITE, COM1, max_days_str, 3);
                    sys_req(WRITE, COM1, ": ", 4);
                } else {
                    valid_day = 1;
                }
            }

            setDate(day, month, year);
        }

        //Get Date command
         else if (!strcmp(buff, "Get Date") || !strcmp(buff, "get date") || !strcmp(buff, "getdate") || !strcmp(buff, "GetDate")){
            getDate();
         }

        // else if((!strcmp(buff, "Create PCB")|| !strcmp(buff, "create PCB") || !strcmp(buff, "CreatePCB") || !strcmp(buff, "createPCB"))){

        //     sys_req(WRITE, COM1, "Enter the process name:\n", 24);
        //     int valid_class = 0;
        //     char pcb_name[20];
        //     char cl[3];
        //     char prior[3];
        //     int class = -1;
        //     int priority = -1;
        //     sys_req(READ, COM1, pcb_name, sizeof(pcb_name) - 1);
        //     int sizerest = 0;
        //     for(int i = 0; i < 9; i++)
        //     {
        //         if(pcb_name[i] == '\0')
        //         {
        //             sizerest = 1;
        //         }
        //     }
        //     if(sizerest == 1)
        //     {
        //     trim_newline(pcb_name);  // Trim the newline character from the input
        //     if(pcb_find(pcb_name) == NULL)
        //     {
                
        //         while(valid_class == 0)
        //         {
        //             sys_req(WRITE, COM1, "Enter the process class (0 for SYSTEM or 1 for USER):\n", 55);
        //             sys_req(READ, COM1, cl, 3);
        //             trim_newline(cl);  // Trim the newline character from the input
        //             class = atoi(cl);
        //             if(!(class == 0 || class == 1)){
        //                 sys_req(WRITE,COM1,"Invalid Class\n", 15);
        //             }else{
        //                 valid_class = 1;
        //             }
        //         }
        //         if(class == 0 || class == 1)
        //         {
        //             int valid_priority = 0;
        //             while(valid_priority == 0 )
        //             {
        //                 sys_req(WRITE, COM1, "Enter the process priority:\n", 29);
        //                 sys_req(READ, COM1, prior, 3);
        //                 trim_newline(prior);  // Trim the newline character from the input
        //                 priority = atoi(prior);
        //                 if(!(priority >= 0 && priority <=9))
        //                 {   
        //                     sys_req(WRITE, COM1, "Error, Please enter a digit 0-9:\n", 34);
        //                 }
        //                 else{
        //                     valid_priority = 1;
        //                 }
        //             }
        //             pcb* new_pcb = pcb_setup(pcb_name, class, priority);
        //             if (new_pcb != NULL){
        //                 pcb_insert(new_pcb);
        //                 sys_req(WRITE, COM1, "Process created and inserted successfully\n", 42);
        //             } 
                   
        //         }
                
        //     }
        //     else{
        //         sys_req(WRITE, COM1, "\nName already exists or is invalid\n", 36);
        //     }
        //     }
        //     else{
        //         sys_req(WRITE, COM1, "\nName already exists or is invalid\n", 36);
        //     }
        // }
        else if(!strcmp(buff, "Delete PCB")|| !strcmp(buff, "delete PCB") || !strcmp(buff, "DeletePCB") || !strcmp(buff, "deletePCB")){
            
            sys_req(WRITE, COM1, "Enter the process name:\n", 24);
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            trim_newline(process_name);  // Trim the newline character from the input
            pcb* target_pcb = pcb_find(process_name);

            if((target_pcb != NULL && process_name[sizeof(process_name) - 1] == '\0') && (target_pcb->Class != SYSTEM)){
            
                pcb_remove(target_pcb);
                sys_req(WRITE, COM1, "Process removed \n", 18);
                
                int success = pcb_free(target_pcb);
                if( success == 0){   //check success of pcb_free()
                    sys_req(WRITE,COM1,"Memory has been sucessfully freed \n", 36);
                }
                else{
                    sys_req(WRITE,COM1,"Error from pcb_free() \n", 31);
                }
                

            }
            else{
                if(target_pcb->Class == SYSTEM)
                    sys_req(WRITE, COM1, "Error: This is a system process, you cannot remove it manually \n", 65);
                else
                {
                    sys_req(WRITE, COM1, "\nName is invalid or does not exist\n", 36);
                }
            }

        }
        else if(!strcmp(buff, "Block PCB")|| !strcmp(buff, "block PCB") || !strcmp(buff, "BlockPCB") || !strcmp(buff, "blockPCB")){
            sys_req(WRITE, COM1, "Enter the process name:\n", 25);
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            trim_newline(process_name);  // Trim the newline character from the input
            pcb* target_pcb = pcb_find(process_name);
            if(target_pcb != NULL && process_name[sizeof(process_name) - 1] == '\0')
            {
                pcb_remove(target_pcb);
                target_pcb->State_E = BLOCKED;
                
                pcb_insert(target_pcb);
                sys_req(WRITE,COM1, "Process Blocked\n", 17);
            }
            else{
                sys_req(WRITE, COM1, "\nName is invalid or does not exist\n", 36);
            }
        }
        else if(!strcmp(buff, "Unblock PCB")|| !strcmp(buff, "unblock PCB") || !strcmp(buff, "UnblockPCB") || !strcmp(buff, "unblockPCB")){
            sys_req(WRITE, COM1, "Enter the process name:\n", 24);
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            trim_newline(process_name);  // Trim the newline character from the input
            pcb* target_pcb = pcb_find(process_name);
            if(target_pcb != NULL && process_name[sizeof(process_name) - 1] == '\0')
            {
                pcb_remove(target_pcb);
                target_pcb->State_E = READY;
                pcb_insert(target_pcb);
                sys_req(WRITE,COM1, "Process Ready\n", 15);
            }
            else{
                sys_req(WRITE, COM1, "\nName is invalid or does not exist\n", 36);
            }
        }


        else if (!strcmp(buff, "Suspend PCB") || !strcmp(buff, "suspend PCB") || !strcmp(buff, "SuspendPCB") || !strcmp(buff, "suspendPCB")) {
            sys_req(WRITE, COM1, "Enter the process name:\n", 24);
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            trim_newline(process_name);  // Trim the newline character from the input
            pcb* target_pcb = pcb_find(process_name);
            if (target_pcb != NULL) {
                target_pcb->State_D = SUSPENDED;
                sys_req(WRITE, COM1, "Process Suspended\n", 19);
            } else {
                sys_req(WRITE, COM1, "\nName is invalid or does not exist\n", 36);
            }
        }

        else if (!strcmp(buff, "Resume PCB") || !strcmp(buff, "resume PCB") || !strcmp(buff, "ResumePCB") || !strcmp(buff, "resumePCB")) {
            sys_req(WRITE, COM1, "Enter the process name:\n", 24);
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            trim_newline(process_name);  // Trim the newline character from the input
            pcb* target_pcb = pcb_find(process_name);
            if (target_pcb != NULL && target_pcb->State_D == SUSPENDED) {
                target_pcb->State_D = NOT_SUSPENDED;  // Change the state back to READY
                sys_req(WRITE, COM1, "Process Resumed\n", 17);
            } else {
                sys_req(WRITE, COM1, "\nName is invalid, does not exist, or is not suspended\n", 55);
            }
        }
        // Set PCB Priority
        else if (!strcmp(buff, "Set PCB Priority") || !strcmp(buff, "SetPCBPriority") || !strcmp(buff, "setPCBpriority") || !strcmp(buff, "set PCB priority")) {
            sys_req(WRITE, COM1, "Enter the process name:\n", 24);
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            trim_newline(process_name);  // Trim the newline character from the input
            pcb* target_pcb = pcb_find(process_name);
            if (target_pcb != NULL) {
                int valid_priority = 0;

                //verify application process 
                if(target_pcb->Class == SYSTEM)
                {
                    sys_req(WRITE, COM1, "Error, this is a System level process, you cannot change the priority.\n", 72);
                    valid_priority = 1;

                }
                while(valid_priority == 0 )
                {
                    char prior[3];
                    sys_req(WRITE, COM1, "Enter the new process priority (0-9):\n", 39);
                    sys_req(READ, COM1, prior, 3);
                    trim_newline(prior);  // Trim the newline character from the input
                    int priority = atoi(prior);
                    if(!(priority >= 0 && priority <=9))
                    {   
                        sys_req(WRITE, COM1, "Error, Please enter a digit 0-9:\n", 34);
                    }
                    else{
                        target_pcb->Priority = priority;  // Sets the new priority

                        sys_req(WRITE, COM1, "Success, new priority updated as: ", 34);

                        //convert for printing
                        char newPrior[4];
                        itoa(target_pcb->Priority, newPrior);

                        //print new priority
                        sys_req(WRITE, COM1, newPrior, 4);
                        sys_req(WRITE, COM1, "\n\n", 3);


                        valid_priority = 1;
                    }
                }
            } else {
                sys_req(WRITE, COM1, "\nName is invalid or does not exist\n", 36);
            }
        }

         else if(!strcmp(buff, "ShowPCB")|| !strcmp(buff, "showPCB") || !strcmp(buff, "Show PCB") || !strcmp(buff, "show PCB"))
        {
          sys_req(WRITE, COM1, "Enter the process name:\n", 24);
          //read and store user inpuFt
            char process_name[9];
            sys_req(READ, COM1, process_name, sizeof(process_name) - 1);
            // Trim the newline character from the input
            trim_newline(process_name);
            //find the pcb
            pcb* target_pcb = pcb_find(process_name);
            //if the pcb is found, display the pcb information
            if (target_pcb != NULL) {
                char class_str[4];
                itoa(target_pcb->Class, class_str);
                char Exe_str[4];
                itoa(target_pcb->State_E, Exe_str);
                char Dis_str[4];
                itoa(target_pcb->State_D, Dis_str);
                char Prio_str[4];
                itoa(target_pcb->Priority, Prio_str);
                sys_req(WRITE, COM1, "\nProcess Name: ", 16);
                sys_req(WRITE, COM1, target_pcb->Name, 9);
                sys_req(WRITE, COM1, "\nProcess Class: ", 17);
                if(target_pcb->Class == 0)
                {
                    sys_req(WRITE, COM1, "System", 6);
                }
                else
                {
                    sys_req(WRITE, COM1, "User", 4);
                }
                sys_req(WRITE, COM1, "\nExecution State: ", 19);
                if(target_pcb->State_E == 0)
                {
                    sys_req(WRITE, COM1, "Ready", 5);
                }
                else {
                    sys_req(WRITE, COM1, "Blocked", 7);
                }
                sys_req(WRITE, COM1, "\nDispatching state: ", 21);
                if(target_pcb->State_D == 0)
                {
                    sys_req(WRITE, COM1, "Not Suspended", 14);
                }
                else {
                    sys_req(WRITE, COM1, "Suspended", 9);
                }
                sys_req(WRITE, COM1, "\nProcess Priority: ", 20);
                sys_req(WRITE, COM1, Prio_str, 2);
                sys_req(WRITE, COM1, "\n", 2);

                

            }
            //if the pcb is not found, display an error message
            else{
                sys_req(WRITE, COM1, "\nName is invalid or does not exist\n", 36);
            }
         }
         //show all ready processes
         else if(!strcmp(buff, "Showready")|| !strcmp(buff, "showready") || !strcmp(buff, "Show ready") || !strcmp(buff, "show ready"))
        {
             //display all ready processes
            sys_req(WRITE, COM1, "Showing Ready Processes...\n", 28);
            node* tempNode = readyQueue->frontPtr;
            while(tempNode != NULL)
            {
                //display the name of the process
                char class_str[4];
                itoa(tempNode->data->Class, class_str);
                char Exe_str[4];
                itoa(tempNode->data->State_E, Exe_str);
                char Dis_str[4];
                itoa(tempNode->data->State_D, Dis_str);
                char Prio_str[4];
                itoa(tempNode->data->Priority, Prio_str);
                sys_req(WRITE, COM1, "Process Name: ", 15);
                sys_req(WRITE, COM1, tempNode->data->Name, 9);
                sys_req(WRITE, COM1, ", Process Class: ", 18);
                if(tempNode->data->Class == 0)
                {
                    sys_req(WRITE, COM1, "System, ", 9);
                }
                else
                {
                    sys_req(WRITE, COM1, "User, ", 7);
                }
                sys_req(WRITE, COM1, "Execution State: ", 18);
                if(tempNode->data->State_E == 0)
                {
                    sys_req(WRITE, COM1, "Ready, ", 8);
                }
                else {
                    sys_req(WRITE, COM1, "Blocked, ", 10);
                }
                sys_req(WRITE, COM1, "Dispatching state: ", 20);
                if(tempNode->data->State_D == 0)
                {
                    sys_req(WRITE, COM1, "Not Suspended, ", 16);
                }
                else {
                    sys_req(WRITE, COM1, "Suspended, ", 12);
                }
                sys_req(WRITE, COM1, "Process Priority: ", 19);
                sys_req(WRITE, COM1, Prio_str, 2);
                sys_req(WRITE, COM1, ".\n", 3);
                //move to the next node
                tempNode = tempNode->nextPtr;
            }

         }
         //show all blocked processes
         else if(!strcmp(buff, "Showblocked")|| !strcmp(buff, "showblocked") || !strcmp(buff, "Show blocked") || !strcmp(buff, "show blocked"))
        {
             //display all blocked processes
            sys_req(WRITE, COM1, "Showing Blocked Processes...\n", 30);
            node* tempNode = blockedQueue->frontPtr;
            //  while the next node is not null
            while(tempNode != NULL)
            {
                //display the name of the process
                char class_str[4];
                itoa(tempNode->data->Class, class_str);
                char Exe_str[4];
                itoa(tempNode->data->State_E, Exe_str);
                char Dis_str[4];
                itoa(tempNode->data->State_D, Dis_str);
                char Prio_str[4];
                itoa(tempNode->data->Priority, Prio_str);
                sys_req(WRITE, COM1, "Process Name: ", 15);
                sys_req(WRITE, COM1, tempNode->data->Name, 9);
                sys_req(WRITE, COM1, ", Process Class: ", 18);
                if(tempNode->data->Class == 0)
                {
                    sys_req(WRITE, COM1, "System, ", 9);
                }
                else
                {
                    sys_req(WRITE, COM1, "User, ", 7);
                }
                sys_req(WRITE, COM1, "Execution State: ", 18);
                if(tempNode->data->State_E == 0)
                {
                    sys_req(WRITE, COM1, "Ready, ", 8);
                }
                else {
                    sys_req(WRITE, COM1, "Blocked, ", 10);
                }
                sys_req(WRITE, COM1, "Dispatching state: ", 20);
                if(tempNode->data->State_D == 0)
                {
                    sys_req(WRITE, COM1, "Not Suspended, ", 16);
                }
                else {
                    sys_req(WRITE, COM1, "Suspended, ", 12);
                }
                sys_req(WRITE, COM1, "Process Priority: ", 19);
                sys_req(WRITE, COM1, Prio_str, 2);
                sys_req(WRITE, COM1, ".\n", 3);
                //move to the next node
                tempNode = tempNode->nextPtr;
            }
         }
         //show all processes
         else if(!strcmp(buff, "Showall")|| !strcmp(buff, "showall") || !strcmp(buff, "Show all") || !strcmp(buff, "show all"))
        {
            sys_req(WRITE, COM1, "Showing All Processes...\n", 26);
            //display all ready processes
            node*tempNode;
            if(readyQueue != NULL){
            tempNode = readyQueue->frontPtr;
            while(tempNode != NULL)
            {
                //display the name of the process
                char class_str[4];
                itoa(tempNode->data->Class, class_str);
                char Exe_str[4];
                itoa(tempNode->data->State_E, Exe_str);
                char Dis_str[4];
                itoa(tempNode->data->State_D, Dis_str);
                char Prio_str[4];
                itoa(tempNode->data->Priority, Prio_str);
                sys_req(WRITE, COM1, "Process Name: ", 15);
                sys_req(WRITE, COM1, tempNode->data->Name, 9);
                sys_req(WRITE, COM1, ", Process Class: ", 18);
                if(tempNode->data->Class == 0)
                {
                    sys_req(WRITE, COM1, "System, ", 9);
                }
                else
                {
                    sys_req(WRITE, COM1, "User, ", 7);
                }
                sys_req(WRITE, COM1, "Execution State: ", 18);
                if(tempNode->data->State_E == 0)
                {
                    sys_req(WRITE, COM1, "Ready, ", 8);
                }
                else {
                    sys_req(WRITE, COM1, "Blocked, ", 10);
                }
                sys_req(WRITE, COM1, "Dispatching state: ", 20);
                if(tempNode->data->State_D == 0)
                {
                    sys_req(WRITE, COM1, "Not Suspended, ", 16);
                }
                else {
                    sys_req(WRITE, COM1, "Suspended, ", 12);
                }
                sys_req(WRITE, COM1, "Process Priority: ", 19);
                sys_req(WRITE, COM1, Prio_str, 2);
                sys_req(WRITE, COM1, ".\n", 3);
                //move to the next node
                tempNode = tempNode->nextPtr;
            }
            }
            //display all blocked processes
            if(blockedQueue != NULL)
            {
            tempNode = blockedQueue->frontPtr;

             while(tempNode != NULL)
            {
                //display the name of the process
                char class_str[4];
                itoa(tempNode->data->Class, class_str);
                char Exe_str[4];
                itoa(tempNode->data->State_E, Exe_str);
                char Dis_str[4];
                itoa(tempNode->data->State_D, Dis_str);
                char Prio_str[4];
                itoa(tempNode->data->Priority, Prio_str);
                sys_req(WRITE, COM1, "Process Name: ", 15);
                sys_req(WRITE, COM1, tempNode->data->Name, 9);
                sys_req(WRITE, COM1, ", Process Class: ", 18);
                if(tempNode->data->Class == 0)
                {
                    sys_req(WRITE, COM1, "System, ", 9);
                }
                else
                {
                    sys_req(WRITE, COM1, "User, ", 7);
                }
                sys_req(WRITE, COM1, "Execution State: ", 18);
                if(tempNode->data->State_E == 0)
                {
                    sys_req(WRITE, COM1, "Ready, ", 8);
                }
                else {
                    sys_req(WRITE, COM1, "Blocked, ", 10);
                }
                sys_req(WRITE, COM1, "Dispatching state: ", 20);
                if(tempNode->data->State_D == 0)
                {
                    sys_req(WRITE, COM1, "Not Suspended, ", 16);
                }
                else {
                    sys_req(WRITE, COM1, "Suspended, ", 12);
                }
                sys_req(WRITE, COM1, "Process Priority: ", 19);
                sys_req(WRITE, COM1, Prio_str, 2);
                sys_req(WRITE, COM1, ".\n", 3);
                //move to the next node
                tempNode = tempNode->nextPtr;
            }
            }
         }
        /* else if(!strcmp(buff, "Yield")|| !strcmp(buff, "yield"))
        {
            sys_req(IDLE);
            sys_req(WRITE, COM1, "CPU yielded.\n", 14);
        } */
        else if (!strcmp(buff, "Load R3") || !strcmp(buff, "load r3")) 
        {
        pcb* pcb1 = pcb_setup("Proc1", USER, 3);
        pcb* pcb2 = pcb_setup("Proc2", USER, 3);
        pcb* pcb3 = pcb_setup("Proc3", USER, 4);
        pcb* pcb4 = pcb_setup("Proc4", USER, 4);
        pcb* pcb5 = pcb_setup("Proc5", USER, 4);

        if (pcb1 && pcb2 && pcb3 && pcb4 && pcb5) {
            // Initialize and save context for each process
            pcb1->StackPtr = (void*)((unsigned int)pcb1->Stack + sizeof(pcb1->Stack) - sizeof(context_t));
            pcb2->StackPtr = (void*)((unsigned int)pcb2->Stack + sizeof(pcb2->Stack) - sizeof(context_t));
            pcb3->StackPtr = (void*)((unsigned int)pcb3->Stack + sizeof(pcb3->Stack) - sizeof(context_t));
            pcb4->StackPtr = (void*)((unsigned int)pcb4->Stack + sizeof(pcb4->Stack) - sizeof(context_t));
            pcb5->StackPtr = (void*)((unsigned int)pcb5->Stack + sizeof(pcb5->Stack) - sizeof(context_t));

            context_t* ctx1 = (context_t*)pcb1->StackPtr;
            context_t* ctx2 = (context_t*)pcb2->StackPtr;
            context_t* ctx3 = (context_t*)pcb3->StackPtr;
            context_t* ctx4 = (context_t*)pcb4->StackPtr;
            context_t* ctx5 = (context_t*)pcb5->StackPtr;

            
            ctx1->eip = (unsigned int)proc1;
            ctx2->eip = (unsigned int)proc2;
            ctx3->eip = (unsigned int)proc3;
            ctx4->eip = (unsigned int)proc4;
            ctx5->eip = (unsigned int)proc5;

            
            //array with all context structs
            context_t* contexts[] = { ctx1, ctx2, ctx3, ctx4, ctx5};
            

            contexts[0]->esp = (int)pcb1->StackPtr;
            contexts[0]->ebp = (int)pcb1->Stack;
            contexts[1]->esp = (int)pcb2->StackPtr;
            contexts[1]->ebp = (int)pcb2->Stack;
            contexts[2]->esp = (int)pcb3->StackPtr;
            contexts[2]->ebp = (int)pcb3->Stack;
            contexts[3]->esp = (int)pcb4->StackPtr;
            contexts[3]->ebp = (int)pcb4->Stack;
            contexts[4]->esp = (int)pcb5->StackPtr;
            contexts[4]->ebp = (int)pcb5->Stack;

            
            //assign all other regs for each context

            for (int i = 0; i < 5; i++){

                contexts[i]->cs = 0x08;
                contexts[i]->ss = contexts[i]->ds = contexts[i]->es = contexts[i]->fs = contexts[i]->gs = contexts[i]->eax =  0x10;
                contexts[i]->ebx = contexts[i]->ebx = contexts[i]->ecx = contexts[i]->edx = contexts[i]->esi = contexts[i]->edi = 0x10;                
                contexts[i]->eflags = 0x0202;
            }

            // Add processes to the ready queue
            pcb_insert(pcb1);
            pcb_insert(pcb2);
            pcb_insert(pcb3);
            pcb_insert(pcb4);
            pcb_insert(pcb5);

            sys_req(WRITE, COM1, "R3 test processes loaded and enqueued.\n", 40);
        } else {
            sys_req(WRITE, COM1, "Failed to allocate PCBs for R3 test processes.\n", 48);
        }
        }
        else if (!strcmp(buff, "Alarm") || !strcmp(buff, "alarm")){
            int valid_hour = 0;
            int valid_min = 0;
            //int valid_sec = 0;
            char hr[20] = {0};
            char min[3] = {0};
            //char sec[3] = {0};
            int hour = 0;
            int second = 0;
            int minute = 0;
            char name [20] = {0};
            char msg [100] = {0};
            
            
            
            
            sys_req(WRITE, COM1, "Please enter a name for the alarm\n", 35);
            sys_req(READ, COM1, name, sizeof(alarm));
                
            sys_req(WRITE, COM1, "Enter a message for your alarm: ", 33);
            sys_req(READ, COM1, msg, sizeof(msg));
            
            sys_req(WRITE, COM1, "Please enter a time for the alarm.\n", 36);
            sys_req(WRITE, COM1, "Hour (0-24): ", 14);
            while(valid_hour == 0){

                sys_req(READ, COM1, hr, 3);


                //check if hr is all valid digits 0-9
                if ( isNum(hr)){

                    hour = atoi(hr);
                    if(hour < 0 || hour > 24)
                        {
                            sys_req(WRITE, COM1, "\nInvalid time. Please enter a number between 0 and 24: \n", 57);
                        } 
                    else
                    {
                        valid_hour = 1;
                    }

                trim_newline(hr);  // Trim the newline character from the input
                hour = atoi(hr);
                if(hour < 0 || hour > 24){
                sys_req(WRITE, COM1, "Invalid time. Please enter a number between 0 and 24: ", 55);
                
                } else{
                    valid_hour = 1;

                }
                }
                else //if isNum() returns false, write invalid char
                {
                    sys_req(WRITE, COM1, "\nInvalid character. Please enter a valid digit between 0 and 24: \n", 67);
                    //memset(buff, 0, sizeof(buff));
                }

            }
            sys_req(WRITE, COM1, "Minute (0-59): ", 16);
            while(valid_min == 0){
                sys_req(READ, COM1, min, 3);
                trim_newline(min);  // Trim the newline character from the input
                minute = atoi(min);
                if(minute < 0 || minute > 59){
                sys_req(WRITE, COM1, "Invalid time. Please enter a number between 0 and 59: \n", 56);
                
                } else{
                    valid_min = 1;
                }
            }
            //  sys_req(WRITE, COM1, "Second (0-59): ", 16);
            // while(valid_sec == 0){
            //     sys_req(READ, COM1, sec, 3);
            //     trim_newline(sec);  // Trim the newline character from the input
            //     second = atoi(sec);
            //     if(second < 0 || second > 59){
            //     sys_req(WRITE, COM1, "Invalid time. Please enter a number between 0 and 59: \n", 56);
                
            //     } else{
            //         valid_sec = 1;
            //     }
            // }
            
            create_alarm(hour, minute, second, name, msg);

        }

           //Start R5 User Commands
        else if ( !(strcmp(buff, "AM")) || !(strcmp(buff, "Allocate Memory")) || !(strcmp(buff, "allocate memory")) || !(strcmp(buff, "allocateMemory")) || !(strcmp(buff, "AllocateMemory"))){
            sys_req(WRITE, COM1, "Please enter the size of desired memory block:\n ", 47);
            int valid_digit = 0;
            int size = 0;


            while( ! valid_digit ){
                char readSize[6] = {0};
                sys_req(READ, COM1, readSize, 6);
                trim_newline(readSize);
                size = atoi(readSize);
                if(size > 0 && size < 49900) // VALID SIZE?
                    {
                        valid_digit = 1; //  Exit loop
                    } 
                    else{   //out of range
                    
                        sys_req(WRITE, COM1, "\nInvalid size. Please enter a number between 0 and 49000:\n", 59);
                        }
            }
            //return starting 
            startAddress = allocate_memory((size_t)size);
            if (startAddress == NULL)
            {
                sys_req(WRITE, COM1, "Memory allocation failed\n",26);
                continue;
            }
            
            // int startAddressStr = (int)(startAddress);
            // int startAddressStr = (int)(startAddress);
            
            // char* charstr = " ";
            // itoa(startAddressStr, charstr);
            // char* charstr = " ";
            // itoa(startAddressStr, charstr);
            sys_req(WRITE,COM1,"Memory allocated at address: ", 30);
            printHex((unsigned int)startAddress);
            // sys_req(WRITE,COM1, charstr, 10);
            printHex((unsigned int)startAddress);
            // sys_req(WRITE,COM1, charstr, 10);
            sys_req(WRITE,COM1, newline, 2);

        }
        else if (!strcmp(buff, "FM") || !strcmp(buff, "Free Memory") || !strcmp(buff, "free memory") || !strcmp(buff, "FreeMemory") || !strcmp(buff, "freeMemory")){
             // free_memory_command();
            

            int integerInput = 0;
        
                sys_req(WRITE, COM1, "Please enter adress of the desired memory block:\n", 50);

                char readAddress[50] = {0};
                sys_req(READ, COM1, readAddress, 50);
                trim_newline(readAddress);
                const char* hexToIntStr = (const char*)readAddress;
              

                integerInput = reverseHex(hexToIntStr);
   

            // char str[11] = {0};
            // itoa(integerInput, str);
            sys_req(WRITE,COM1,"Attempting to free at address: ", 32);
            sys_req(WRITE,COM1, newline, 2);
            sys_req(WRITE,COM1, hexToIntStr, 11);
            sys_req(WRITE,COM1, newline, 2);
            int valid = free_memory((void*) integerInput);
            if(valid == 0)
            {
                sys_req(WRITE,COM1,"Memory block freed successfully\n", 33);
            }
            else if (valid == 1)
            {
                sys_req(WRITE,COM1,"Block not found in free_memory()\n", 34);
                sys_req(WRITE,COM1,"Block not found in free_memory()\n", 34);
            }
            else if (valid == 2)
            {
                sys_req(WRITE,COM1,"free list error\n", 17);
            }
        }
        else if ( (!(strcmp(buff, "SAM"))) ||  !(strcmp(buff, "Show Allocated Memory")) || !(strcmp(buff, "show allocated memory")) || !(strcmp(buff, "ShowAllocatedMemory")) || !(strcmp(buff, "showAllocatedMemory"))){
            show_allocated_memory();
        }
        else if (!strcmp(buff, "SFM") || !strcmp(buff, "Show Free Memory") || !strcmp(buff, "show free memory") || !strcmp(buff, "ShowFreeMemory") || !strcmp(buff, "showFreeMemory")){
             show_free_memory();
        }
        else if(!strcmp(buff,"SA") || !strcmp(buff,"show all") || !strcmp(buff,"Show All") || !strcmp(buff,"Show all") || !strcmp(buff,"showall") || !strcmp(buff,"ShowAll")){
            //show all processes
            show_allocated_memory();
            sys_req(WRITE,COM1, newline,2);
            sys_req(WRITE,COM1, newline,2);

            sys_req(WRITE,COM1, "Beginning of free list:\n",25);

            show_free_memory();
        }
        else if (!strcmp(buff, "")) {

        }
        else if (!strcmp(buff, "")) {

        }
         else{
            sys_req(WRITE, COM1, "Invalid Command, Type \"Help\" for a list of commands\n", 53);
         }

            sys_req(IDLE);
        
    }
}
void custom_itoa(int num, char* str) {
    int i = 0;
    while (num > 0) {
        str[i++] = num % 10 + '0';
        num /= 10;
    }
    str[i] = '\0';

    // Reverse string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void show_allocated_memory(void) { //print each block in the allocated list's data

            //get head pointer to allocated list
            mcb* tempMcb = alloc_list->tailPtr; //need to free this to prevent memory leaks at end
            //while the next node is not null
            int blockCount = 1;
            char* blockCountStr = " ";
            //int nodesChecked = 0;
            while(tempMcb != NULL )
            {                
                if(tempMcb->flag == 1){
                if(tempMcb->Size != 0){
                //print the name of the block
                sys_req(WRITE, COM1, "\n", 2);

                sys_req(WRITE, COM1, "\n", 2);

                sys_req(WRITE, COM1, "Allocated Block ", 17);
                itoa(blockCount, blockCountStr);
                sys_req(WRITE, COM1, blockCountStr, strlen(blockCountStr));
                sys_req(WRITE, COM1, ":", 2);
                sys_req(WRITE, COM1, "\n", 2);
                sys_req(WRITE, COM1, "\n", 2);

                //print the start address and size of allocated mcb block
                // char* startAddressStr = (char*) (tempMcb)->Start_addr;
                // char* charStr = " ";
                // int startAddressInt = (int)((tempMcb->Start_addr));
                // itoa(startAddressInt, charStr);
                // char* charStr = " ";
                // int startAddressInt = (int)((tempMcb->Start_addr));
                // itoa(startAddressInt, charStr);
                // startAddressStr = tempMcb->Start_addr;


                sys_req(WRITE, COM1, "Start Address: ", 16);

                printHex((unsigned int)tempMcb->Start_addr);
            
                
                // sys_req(WRITE, COM1, charStr, 10);
                char* sizeStr = " ";
                // sys_req(WRITE, COM1, "\n", 2);
                itoa((int)(tempMcb->Size), sizeStr);
                // sizeStr = tempMcb->Size;
                sys_req(WRITE, COM1, "Size of block: ", 16);
                
                
                sys_req(WRITE, COM1, sizeStr, strlen(sizeStr));
                sys_req(WRITE, COM1, "\n", 2);
                sys_req(WRITE, COM1, "\n", 2);
                blockCount++;
                }
                }
                //move to the next node
                tempMcb = tempMcb->PrevPtr;
                //nodesChecked++;
                
            }
            sys_req(WRITE, COM1, "End of Allocated Memory list\n", 30);
}


void show_free_memory(void){

    //print each block in the free list's data

            //get head pointer to free list
            mcb* tempMcb = alloc_list->headPtr;
            //while the next node is not null
            int blockCount = 1;
            char* blockCountStr = " ";
            

            while(tempMcb != NULL)
            {
                 //print the start address and size of allocated mcb block
                // char* startAddressStr = (char*) (tempMcb)->Start_addr;
                
                // int startAddressInt = (int)((tempMcb->Start_addr));
                // char charStr[10];
                // int startAddressInt = (int)((tempMcb->Start_addr));
                // char charStr[10];
                //void* SA = (void*)tempMcb->Start_addr;
                // itoa(startAddressInt, charStr);
                // itoa(startAddressInt, charStr);
                //print the name of the block
                if(tempMcb->flag == 0)
                {
                sys_req(WRITE, COM1, "\n", 2);

                sys_req(WRITE, COM1, "Free Block: ", 13);
                itoa(blockCount, blockCountStr);
                sys_req(WRITE, COM1, blockCountStr, strlen(blockCountStr) + 1);
                
                sys_req(WRITE, COM1, ":", 2);
                sys_req(WRITE, COM1, "\n", 2);
                sys_req(WRITE, COM1, "\n", 2);
                // startAddressStr = tempMcb->Start_addr;
                sys_req(WRITE, COM1, "Start Address: ", 16);
                // sys_req(WRITE, COM1, charStr, 10);
                printHex((unsigned int)tempMcb->Start_addr);
                // sys_req(WRITE, COM1, charStr, 10);
                printHex((unsigned int)tempMcb->Start_addr);
                char* sizeStr = " ";

                // sys_req(WRITE, COM1, "\n", 2);
                // sys_req(WRITE, COM1, "\n", 2);

                itoa((int)(tempMcb->Size), sizeStr);
                // sizeStr = tempMcb->Size;

                sys_req(WRITE, COM1, "Size of block: ", 16);
                sys_req(WRITE, COM1, sizeStr, strlen(sizeStr) + 1);
                sys_req(WRITE, COM1, "\n", 2);
                sys_req(WRITE, COM1, "\n", 2);

                //move to the next node
                
                
                }
                tempMcb = (void*)tempMcb->NextPtr;
            }

            sys_req(WRITE, COM1, "End of Free Memory list\n", 25);
}



void printHex(unsigned int num) {
    char hex[16] = "0123456789ABCDEF";
    char buffer[9]; // Buffer for the hexadecimal string (assuming 32-bit integer)
    buffer[8] = '\0'; // Null-terminate the string

    // Convert each nibble to its hexadecimal representation
    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex[num & 0xF]; // Mask the lowest nibble and convert to hexadecimal
        num >>= 4; // Move to the next nibble
    }

    // Print the hexadecimal string
    sys_req(WRITE, COM1, buffer, strlen(buffer));
    sys_req(WRITE, COM1, "\n", 2);
}

unsigned int reverseHex(const char* hexString) {
    unsigned int num = 0;
    for (int i = 0; hexString[i] != '\0'; i++) {
        num <<= 4; // Shift left by 4 bits to make room for the new nibble
        if (hexString[i] >= '0' && hexString[i] <= '9') {
            num += hexString[i] - '0'; // Convert digit character to integer
        } else if (hexString[i] >= 'A' && hexString[i] <= 'F') {
            num += hexString[i] - 'A' + 10; // Convert uppercase hexadecimal character to integer
        } else if (hexString[i] >= 'a' && hexString[i] <= 'f') {
            num += hexString[i] - 'a' + 10; // Convert lowercase hexadecimal character to integer
        } else {
            // Invalid character
            return 0;
        }
    }
    return num;
}

