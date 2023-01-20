//
// Created by Nate Harper on 1/18/2023.
//
#include <mpx/io.h>
#include <mpx/device.h>

#define IndexRegister 0x70
#define DataRegister 0x71

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
//    char buf[11] = {0};
//
      outb(IndexRegister,Month);
      char* month = &inb(DataRegister);
      outb(COM1,month);
//
//    outb(IndexRegister, DayOfMonth);
//    byte day = inb(DataRegister);
//
//    buf = month + day;
//    return &buf;
    return month;
}
