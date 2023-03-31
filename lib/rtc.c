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
#include <stdio.h>

#define IndexRegister 0x70
#define DataRegister 0x71

int MonthDays[] = {0,
                  31,//January
                  28,//February
                  31,//March
                  30,//April
                  31,//May
                  30,//June
                  31,//July
                  31,//August
                  30,//September
                  31,//October
                  30,//November
                  31,//December
                  };

int read();
void formatTime(int timeToFormat, char* stringPtr);

//typedef containg list of all ports to use
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

/*
Function Name   : getDate
Function Desc   : reads date from ports, converts from BCD to decimal, and returns as char*

@return char* : formatted char* of the received date
*/
char* getDate(){
    char* buf = (char*)sys_alloc_mem(10);
    sprintf(buf, "%.2d/%.2d/%.2d",read(Month),read(DayOfMonth),read(Year));
    return buf;
}

/**
@name   : setDate
@brief  : takes parameter newDate and writes to ports to set new date

@param newDate (char*) : char formatted as MM/DD/YYYY
*/
void setDate(char* newDate){
    int date[3];
    for (int i = 0, seek = 0; newDate[seek] != '\0'; seek++, i++) {
        char* temp = (char*)sys_alloc_mem(3);
        for (int p = 0; ; seek++, p++) {
            if (newDate[seek] == '/'|| newDate[seek] == '\0') {
                temp[p] = '\0';
                date[i] = atoi(temp);
                break;
            } else {
                temp[p] = newDate[seek];
            }
        }
        sys_free_mem(temp);
    }
    if(date[0]<=0||date[0]>12) {
        puts("\n\n\e[0;91m$:Invalid Month, reverting to previous date.\e[0m");
        return;
    }
        if(date[2]<0){

            puts("\n\n\e[0;91m$:Invalid day, reverting to previous date.\e[0m");
    return;
}
        if(date[1]<=0){
            puts("\n\n\e[0;91m$:Invalid month, reverting to previous date.\e[0m");
    }
    if(date[1]>MonthDays[date[0]]){
        if(!(date[0] == 2 && date[1] == 29 && date[2] %4 ==0)){
            puts("\n\n\e[0;91m$:Date does not exist, reverting to previous date.\e[0m");
            return;
        }
    }
    write(Year,date[2]);
    if(bcdToDec(date[1])>MonthDays[read(Month)]){
        write(Month,date[0]);
        write(DayOfMonth,date[1]);
    }
    else {
        write(DayOfMonth, date[1]);
        write(Month, date[0]);
    }

    }

/**
Function Name   : getTime
Function Desc   : will get time from ports, convert frm BCD to decimal, and return as a char*
 
@return char* : formatted char* of the recieved time
*/
char* getTime() {
    //Creating buffer char*
    char* buf = (char*)sys_alloc_mem(200);

    //Reading from ports
    int hours = read(Hours);
    int minutes = read(Minutes);
    int seconds = read(Seconds);

    //Creating char* variables for output
    char* hoursOutput = (char*)sys_alloc_mem(50);
    char* minutesOutput = (char*)sys_alloc_mem(50);
    char* secondsOutput = (char*)sys_alloc_mem(50);

    //Formatting time using formatTime
    formatTime(hours, hoursOutput);
    formatTime(minutes, minutesOutput);
    formatTime(seconds, secondsOutput);

    //Using sprintf to form
    sprintf(buf, "%s:%s:%s", hoursOutput, minutesOutput, secondsOutput);

    //Returning buffer
    return buf;
}

/*
Function Name   : formatTime
Function Desc   : take time as int and string pointer, and formats time correctly to be outputted

@param timeToFormat (int) : time that needs to be formatted
@param stringPtr (char*) : pointer used to to save outputted string
*/
void formatTime(int timeToFormat, char* stringPtr) {
    if (timeToFormat == 0) {
        sprintf(stringPtr, "%s", "00");
    } else if (timeToFormat < 10) {
        sprintf(stringPtr, "%s%d", "0", timeToFormat);
    } else {
        sprintf(stringPtr, "%d", timeToFormat);
    }
} 

/**
Function Name   : setTime
Function Desc   : takes parameter of char* and writes to ports to set the new time

@param newTime (char*) : char formatted as HH:MM:SS
*/
void setTime(char* newTime) {
    //Creating char* array containing each hours, minutes, and seconds
    char *time[3] = {(char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3), (char*)sys_alloc_mem(3)};
    
    //for loop to go through newTime and checking each section of the inputed time
    for (int i = 0, seek = 0; newTime[seek] != '\0'; i++, seek++) {
        for (int p = 0 ;; p++, seek++) {
            //Seeing if char looked at is ':' (section divider) of '/0' (end of char)
            if (newTime[seek] == ':' || newTime[seek] == '\0') {
                //Adding ending char for time section
                time[i][p] = '\0';
                break;
            } else {
                //Adding char to time section
                time[i][p] = newTime[seek];
            }
        }
    }

    //Validating that date entered was correct
    int hours = atoi(time[0]);
    int minutes = atoi(time[1]);
    int seconds = atoi(time[2]);

    if (hours >= 24 || hours < 0) {
        //hours is not between 0-23
        puts("\nInvalid time!");
        return;
    } else if (minutes >= 60 || minutes < 0) {
        //minutes is not between 0-59 
        puts("\nInvalid time!");
        return;
    } else if (seconds >= 60 || seconds < 0) {
        //seconds is not beyween 0-59
        puts("\nInvalid time!");
        return;
    }
        
    //Going through each section of new time and writing to each port as time entered is valid
    index indexes[3] = {Hours, Minutes, Seconds};
    for (size_t i = 2; i>0; i--) {
        write(indexes[i], atoi(time[i]));
    }
}

/*
Function Name  : read
Function Desc  : Takes port index, reads from the port, converts recieved binary coded decimal number to a decimal number, and returns value

@param index (int) : the port number
@return int : recieved value converted to decimal
*/
int read(index in){
    outb(IndexRegister, in);
    return bcdToDec(inb(DataRegister));
}

/*
Function Name  : write
Function Desc  : takes parameters of where and what to writes, and writes to the desired port

@param i (enum index) : the port to write to
@param num (int) : the value to be written
*/
void write(enum index i, int num){

    {
        outb(IndexRegister,i);
        outb(DataRegister, decToBcd(num));
    }
}

/*
Function Name  : bcdToDecimal
Function Desc  : converts BCD (binary coded decimal) to decimal and returns value [used to read value from ports]

@param src (int) : binary coded decimal number
@return int : decimal value
*/
int bcdToDec(int src){
    int res = 0;
    for(int i = 12;i>=0;i-=4){
        res*=10;
        res+=((src>>i)&15);
    }
    return res;
}

/*
Function Name  : decToBcd
Function Desc  : converts decimal value to BCD (binary coded decimal) and returns value [used to write value to ports]

@params src (int) : decimal value to convert
@return int : bcd value
*/
int decToBcd(int src){
    int res = (((src/10)&15)<<4)+((src%10)&15);
    return res;
}
