
#ifndef WINDOWS_9_ALARM_H
#define WINDOWS_9_ALARM_H
#define MAX_ALARMNAME_LENGTH 15
#define MAX_ALARMTIME_LENGTH 9

#include <linked-list.h>

typedef struct alarm {
    char* alarmName[MAX_ALARMNAME_LENGTH + 1];
    char* alarmTime[MAX_ALARMTIME_LENGTH + 1];
}alarm;

extern list* alarmList;

struct alarm* create_alarm(char* alarmName, char* time);

struct pcb* load_alarm(void* function);

void run_alarm();

struct alarm* remove_alarm(alarm* alarm);

int time_comparison(char* alarmTime);

char[] seperating_time(char* time);

#endif //WINDOWS_9_ALARM_H