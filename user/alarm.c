#include <alarm.h>
#include <alarm.h>
#include <stdio.h>
#include <string.h>
#include <linked-list.h>
#include <rtc.h>

//creating alarm linked-list
list* alarmList;
int alarmListSize = 0;

struct alarm* create_alarm(char* alarmName, char* time) {
    //splits up time into array
    char* alarmTime[] = seperating_time(time);

    //validating entered time
    int alarmHours = atoi(alarmTime[0]);
    int alarmMinutes = atoi(alarmTime[1]);
    int alarmSeconds = atoi(alarmTime[2]);

    if (alarmHours >= 24 || alarmHours < 0) {
        //hours is not between 0-23
        puts("\nInvalid alarm time!");
        return;
    } else if (alarmMinutes >= 60 || alarmMinutes < 0) {
        //minutes is not between 0-59 
        puts("\nInvalid alarm time!");
        return;
    } else if (alarmSeconds >= 60 || alarmSeconds < 0) {
        //seconds is not beyween 0-59
        puts("\nInvalid alarm time!");
        return;
    }

    //validating alarm name
    if (name == NULL || strlen(alarmName) > MAX_ALARMNAME_LENGTH) {
        puts("\nInvalid alarm name!");
    }

    //checking if alarm list is null
    if (alarmListSize == 0) {
        alarmList = createList();
    }

    /*
                    Check to make sure alarm name does not already exist
    */

    //creating alarm and setting data to 
    alarm* newAlarm = (alarm*)sys_alloc_mem(sizeof(alarm*));
    newAlarm->alarmName = alarmName;
    newAlarm->alarmTime = alarmTime;

    //adding new alarm to list
    add(alarmList, create_node(newAlarm));

    puts("\nAlarm has been created!");
}

struct pcb* load_alarm(void* function) {

}

void run_alarm() {

}

struct alarm* remove_alarm(alarm* alarm) {
    //checking is alarm list exists
    if (alarmList == NULL) {
        puts("Alarm does not exist!");
        return;
    }

    //traversing list to find alarm to remove
    
    

}

int time_comparison(char* alarmTime) {
    char* currentTime = getTime();

}

char[] seperating_time(char* time) {
    char* newTime[3] = {(char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3)};
    for (int i = 0, seek = 0; time[seek] != '\0'; i++, seek++) {
        for (int p = 0 ;; p++, seek++) {
            //Seeing if char looked at is ':' (section divider) of '/0' (end of char)
            if (time[seek] == ':' || time[seek] == '\0') {
                //Adding ending char for time section
                time[i][p] = '\0';
                break;
            } else {
                //Adding char to time section
                newTime[i][p] = time[seek];
            }
        }
    }

    return newTime;
}

