#include <alarm.h>
#include <alarm.h>
#include <stdio.h>
#include <string.h>
#include <linked-list.h>
#include <rtc.h>
#include <memory.h>
#include <stdlib.h>
#include <pcb.h>
#include <mpx/sys_call.h>
#include <sys_req.h>

//creating alarm linked-list
list* alarmList;
int alarmListSize = 0;

struct alarm* createAlarm(char* alarmName, char* time) {
    //splits up time into array
    char *alarmTime[3] = {(char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3)};
    for (int i = 0, seek = 0; time[seek] != '\0'; i++, seek++) {
        for (int p = 0 ;; p++, seek++) {
            //Seeing if char looked at is ':' (section divider) of '/0' (end of char)
            if (time[seek] == ':' || time[seek] == '\0') {
                //Adding ending char for time section
                alarmTime[i][p] = '\0';
                break;
            } else {
                //Adding char to time section
                alarmTime[i][p] = time[seek];
            }
        }
    }

    //validating entered time
    int alarmHours = atoi(alarmTime[0]);
    int alarmMinutes = atoi(alarmTime[1]);
    int alarmSeconds = atoi(alarmTime[2]);

    if (alarmHours > 24 || alarmHours < 0) {
        //hours is not between 0-23
        puts("\nInvalid alarm time!");
        return NULL;
    } else if (alarmMinutes > 60 || alarmMinutes < 0) {
        //minutes is not between 0-59 
        puts("\nInvalid alarm time!");
        return NULL;
    } else if (alarmSeconds > 60 || alarmSeconds < 0) {
        //seconds is not beyween 0-59
        puts("\nInvalid alarm time!");
        return NULL;
    }

    //validating alarm name
    if (alarmName == NULL || strlen(alarmName) > MAX_ALARMNAME_LENGTH) {
        puts("\nInvalid alarm name!");
        return NULL;
    }

    //checking if alarm list is null
    if (alarmListSize == 0) {
        alarmList = createList();
    }

    

    //creating alarm and setting data to 
    alarm* newAlarm = (alarm*)sys_alloc_mem(sizeof(alarm*));
    newAlarm->alarmName = alarmName;
    newAlarm->alarmTime = time;

    //adding new alarm to list and incrementing size
    add(alarmList, createNode(newAlarm));
    alarmListSize++;

    puts("\nAlarm has been created!");

    return newAlarm;
}

struct pcb* loadAlarm(void* function) {
    pcb* alarm = pcb_setup("alarm", USER, 0);
    alarm->dispatchingState = NOT_SUSPENDED;
    alarm->executionState = READY;


    context* con1 = (context*)alarm->stackPtr;
    memset(con1, 0, sizeof(context));
    con1->fs = 0x10;
    con1->gs = 0x10;
    con1->ds = 0x10;
    con1->es = 0x10;
    con1->ss = 0x10;
    con1->CS = 0x08;
    con1->EBP = (int)alarm->stack;
    con1->EIP = (unsigned int) function;
    con1->EFLAGS = 0x0202;

    pcb_insert(alarm);
    return alarm;
}

void runAlarm() {
        node* nodePtr = getHead(alarmList);

        while (nodePtr != NULL) {
            alarm* alarmPtr = (alarm*)nodePtr->data;

            int check = compareTime(alarmPtr->alarmTime);

            if (check == 0) {
                printf("\n\n%s\n\n", alarmPtr->alarmName);
                removeAlarm(alarmPtr);
            }

            nodePtr = nodePtr->nextPtr;            
        }
        sys_req(IDLE);
}


void removeAlarm(alarm* alarm) {
    //checking is alarm list exists
    if (alarmList == NULL) {
        puts("Alarm does not exist!");
        return;
    }

    //traversing list to find alarm to remove
    node* currentPtr;
    for (currentPtr = getHead(alarmList); currentPtr != NULL && currentPtr->data != alarm ; currentPtr = currentPtr->nextPtr);
    
    //checks to see if currentPtr made it to the end of the list
    if (currentPtr == NULL) {
        puts("Alarm does not exist!");
        return;
    } 

    //removes alarm and frees allocated memory
    if (currentPtr->nextPtr == NULL) {
        //currentPtr is at end of list
        currentPtr->prevPtr = NULL;
    } else if (currentPtr->prevPtr == NULL) {
        //currentPtr is at beginning of list
        removeHead(alarmList);
    } else {
        //currentPtr is within list
        currentPtr->prevPtr->nextPtr = currentPtr->nextPtr;
        currentPtr->nextPtr->prevPtr = currentPtr->prevPtr;
    }

    //frees memory allocated for alarm
    sys_free_mem(currentPtr);
}

int compareTime(char* alarmTime) {
    char* currentTime = getTime();

    //seperating get time and changing them to ints
    char *currentTimeArray[3] = {(char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3)};
    extractingTime(currentTimeArray, currentTime);
    int currentHours = atoi(currentTimeArray[0]);
    int currrentMinutes = atoi(currentTimeArray[1]);
    int currentSeconds = atoi(currentTimeArray[2]);
    sys_free_mem(currentTimeArray);

    //seperating alarm time
    char *alarmTimeArray[3] = {(char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3)};
    extractingTime(alarmTimeArray, alarmTime);
    int alarmHours = atoi(alarmTimeArray[0]);
    int alarmMinutes = atoi(alarmTimeArray[1]);
    int alarmSeconds = atoi(alarmTimeArray[2]);
    sys_free_mem(alarmTimeArray);

    //checking to see if alarm should go off or not
    if (currentHours > alarmHours) {
        return 0;
    } else if (currentHours == alarmHours) {
        if (currrentMinutes > alarmMinutes) {
            return 0;
        } else if (currrentMinutes == alarmMinutes) {
            if (currentSeconds > alarmSeconds) {
                return 0;
            } else if (currentSeconds == alarmSeconds) {
                return 0;
            }
        }
    }

    //fallthrough
    return 1;
}

void extractingTime(char* array[], char* time) {
    for (int i = 0, seek = 0; time[seek] != '\0'; i++, seek++) {
        for (int p = 0 ;; p++, seek++) {
            //Seeing if char looked at is ':' (section divider) of '/0' (end of char)
            if (time[seek] == ':' || time[seek] == '\0') {
                //Adding ending char for time section
                array[i][p] = '\0';
                break;
            } else {
                //Adding char to time section
                array[i][p] = time[seek];
            }
        }
    }

}

