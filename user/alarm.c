#include <alarm.h>
#include <string.h>
#include <sys_req.h>
#include <memory.h>
#include <PCB.h>
#include <processes.h>
#include <time_date.h>
#include <stdlib.h>
alarm* headPtr = NULL;

void create_alarm(int hr, int min, int sec, const char* name, const char* msg){

    if(pcb_find("Alarm") == NULL){
        alarm_setup();
    }
    alarm* new_alarm = (alarm*)sys_alloc_mem(sizeof(alarm));

    if(new_alarm == NULL){
        sys_req(WRITE, COM1, "Failed to allocate alarm", 25);
        return;
    }

    new_alarm->hr = hr;
    new_alarm->min = min;
    new_alarm->sec = sec;
    copy_string(new_alarm->name, name, sizeof(new_alarm->name) -1);
    new_alarm->name[sizeof(new_alarm->name) - 1] = '\0';
    copy_string(new_alarm->msg, msg, sizeof(new_alarm->msg) -1);
    new_alarm->msg[sizeof(new_alarm->msg) - 1] = '\0';
    new_alarm->nextPtr = NULL;

    alarm_insert(new_alarm);
}

void alarm_insert(alarm* alarm){
    if(headPtr == NULL){
        headPtr = alarm;
    }else{
        alarm->nextPtr = headPtr;
        headPtr = alarm;
    }
}

void alarm_remove(const char* name){
    alarm* current = headPtr;
    alarm* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                headPtr = current->nextPtr;
            } else {
                prev->nextPtr = current->nextPtr;
            }
            sys_free_mem(current);
            return;
        }
        prev = current;
        current = current->nextPtr;
    }
}

void check_alarm(void)
{
    while(1){
        alarm* temp = NULL;
        int minutes = BCDtoDec(getMinutes());
        int hours = BCDtoDec(getHours());
        //set temp to head pointer
        if(headPtr == NULL){
            sys_req(EXIT);
        }
        temp = headPtr;
        //WHILE temp != Null
        while(temp != NULL){
            

            if(temp->hr < hours || (temp->hr == hours && temp->min < minutes) || (temp->hr == hours && temp->min == minutes)){
                sys_req(WRITE,COM1, "\n", 2);
                sys_req(WRITE,COM1, temp->msg, 100);
                sys_req(WRITE,COM1, "\n", 2);
                //remove alarm from the queue
                alarm_remove(temp->name);
                
            }

            temp = temp->nextPtr;
        }
                sys_req(IDLE);
    }
}
