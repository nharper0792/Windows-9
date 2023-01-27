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
int read();
void write(enum index i, int num);

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


/*
Function Name   : getDate
Function Desc   : reads date from ports, converts from BCD to decimal, and returns as char*

@return char* : formatted char* of the received date
*/
char* getDate(){
    char* buf = (char*)sys_alloc_mem(100);
    sprintf(buf, "%d/%d/%d",read(Month),read(DayOfMonth),read(Year));
    return buf;
}

/*
Function Name   : setDate
Function Desc   : takes parameter newDate and writes to ports to set new date

@param newDate (char*) : char formatted as MM/DD/YYYY
*/
void setDate(char* newDate){
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

/*
Function Name   : getTime
Function Desc   : will get time from ports, convert frm BCD to decimal, and return as a char*
 
@return char* : formatted char* of the recieved time
*/
char* getTime() {
    //Creating buffer char*
    char* buf = (char*)sys_alloc_mem(200);

    //Using sprintf to read from ports and format buffer for return
    sprintf(buf, "%d:%d:%d", read(Hours), read(Minutes), read(Seconds));

    //Returning buffer
    return buf;
}

/*
Function Name  : setTime
Function Desc  : takes parameter of char* and writes to to ports to set the new time

@param newTime (char* newTime) : char formatted as H:M:S
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

    //Going through each section of new time and writing to each port
    index indexes[3] = {Hours, Minutes, Seconds};
    for (size_t i = 0; i<sizeof(time)/sizeof(time[0]); i++) {
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
    sti();
    {
        outb(IndexRegister,i);
        outb(DataRegister, decToBcd(num));
    }
    cli();
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