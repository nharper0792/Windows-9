//
// Created by Nate Harper on 1/18/2023.
//
#include <mpx/io.h>
#include <mpx/device.h>
#include <memory.h>
#include <rtc.h>
#include <stdlib.h>

#define IndexRegister 0x70
#define DataRegister 0x71

int bcdToDec(int src);

enum indexes{
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
};

char* getDate(){
    outb(IndexRegister,Month);

    char* month = (char*)sys_alloc_mem(3);
    itoa(bcdToDec(inb(DataRegister)),month);

    outb(IndexRegister, DayOfMonth);
    char* day = (char*)sys_alloc_mem(3);
    itoa(bcdToDec(inb(DataRegister)),day);

    outb(IndexRegister,Year);
    char* year = (char*)sys_alloc_mem(5);
    itoa(bcdToDec(inb(DataRegister)),year);

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
    return buf;
}


int bcdToDec(int src){
    int res = 0;
    for(int i = 12;i>=0;i-=4){
        res*=10;
        res+=((src>>i)&15);
    }
    return res;
}