
#ifndef WINDOWS_9_ALARM_H
#define WINDOWS_9_ALARM_H
#define MAX_ALARMNAME_LENGTH 15
#define MAX_ALARMTIME_LENGTH 9

#include <linked-list.h>

typedef struct alarm {
    char* alarmName;
    char* alarmTime;
}alarm;

extern list* alarmList;

struct alarm* createAlarm(char* alarmName, char* time);

struct pcb* loadAlarm(void* function);

void runAlarm();

void removeAlarm(alarm* alarm);

int compareTime(char* alarmTime);
void extractingTime(char* array[], char* time);
#endif //WINDOWS_9_ALARM_H