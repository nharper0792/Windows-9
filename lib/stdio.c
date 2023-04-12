//
// Created by Nate Harper on 1/24/2023.
//
#include <stdio.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mpx/device.h>
#include <memory.h>
void printf(char* format,...){
    va_list valist;
    va_start(valist, format);
    char* buffer = formatCore(format, valist);
    va_end(valist);
    puts(buffer);
    sys_free_mem(buffer);
    buffer = NULL;
}
void puts(char* buf){
    sys_req(WRITE,COM1,buf,strlen(buf));
}
