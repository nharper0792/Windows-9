//
// Created by Nate Harper on 1/18/2023.
//
#include <mpx/io.h>
#include <mpx/device.h>
#include <memory.h>
#include <rtc.h>
#include <stdlib.h>
#include <string.h>
#include <mpx/interrupts.h>

#define IndexRegister 0x70
#define DataRegister 0x71

int bcdToDec(int src);
int decToBcd(int src);
char* read();

typedef enum index{
    Seconds = 0x00,
    SecondsAlarm = 0x01,
    Minutes = 0x02,
    MinutesAlarm = 0x03,
    Hours = 0x04,
    HoursAlarm = 0x05,
    DayOfWeek = 0x06,
    DayOfMonth = 0x07,
    Month = 0x08,
    Year = 0x09
}index;
void write(enum index i, int num);



char* getDate(){

    char* month = read(Month);

    char* day = read(DayOfMonth);

    char* year = read(Year);

    char* buf = (char*)sys_alloc_mem(100);
    int p = 0;
    for(int i = 0;month[i]!=0;i++){
        buf[p++] = month[i];
    }
    buf[p++] = '/';
    for(int j = 0;day[j]!=0;j++){
        buf[p++] = day[j];
    }
    buf[p++] = '/';
    for(int k = 0;day[k]!=0;k++){
        buf[p++] = year[k];
    }
   sys_free_mem(month);
    sys_free_mem(day);
    sys_free_mem(year);
    return buf;
}

void setDate(char* newDate){
    char* poop = newDate;
    newDate = poop;
    char *date[3] = {(char*)sys_alloc_mem(3),(char*)sys_alloc_mem(3),(char*)sys_alloc_mem(3)};
    for (int i = 0, seek = 0; newDate[seek] != '\0'; seek++, i++) {
        for (int p = 0; ; seek++, p++) {
            if (newDate[seek] == '/'|| newDate[seek] == '\0') {
                date[i][p] = '\0';
                break;
            } else {
                date[i][p] = newDate[seek];
            }
        }
    }
    index indexes[3] = {Month, DayOfMonth, Year};
    for(size_t i = 0;i<sizeof(date)/sizeof(date[0]);i++){
        write(indexes[i],atoi(date[i]));
    }
}

char* read(index in){
    char* res = (char*)sys_alloc_mem(3);
    outb(IndexRegister, in);
    itoa(bcdToDec(inb(DataRegister)),res);
    return res;
}

void write(enum index i, int num){
    sti();
    {
        outb(IndexRegister,i);
        outb(DataRegister, decToBcd(num));
    }
    cli();
}

int bcdToDec(int src){
    int res = 0;
    for(int i = 12;i>=0;i-=4){
        res*=10;
        res+=((src>>i)&15);
    }
    return res;
}

int decToBcd(int src){
    int res = (((src/10)&15)<<4)+((src%10)&15);
    return res;
}