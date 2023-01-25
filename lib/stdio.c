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
    char buffer[512] = {0};
    char ch;
    int index = 0;

    char *temp_str;
    int temp_int;

    while ((ch = *(format++))) {
        if (ch == '%') {
            switch (ch = *format++) {
                case '%':
                    buffer[index++] = '%';
                    break;
                case 's':
                    temp_str = va_arg(valist, char *);
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
                case 'c':
                    buffer[index++] = va_arg(valist, int);
                    break;
                case 'd':
                case'i':
                    temp_int = va_arg(valist, int);
                    temp_str = itoa(temp_int, NULL);
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
            }
        }

        else {
            buffer[index++] = ch;
        }
    }
    buffer[index] = '\0';
    puts(buffer);
}
void puts(char* str){
    sys_req(WRITE,COM1,str,strlen(str));
}