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

    int padding = 0;
    int pad_with_zeros = 0;
    char *temp_str;
    int temp_int;

    while (ch = *(format++)) {
        if (ch == '%') {
            start:
            switch (ch = *format++) {
                case '%':
                    buffer[index++] = '%';
                    break;
                case 's':
                    temp_str = va_arg(valist, char *);
                    if (padding) {
                        temp_str = pad(temp_str, padding, ' ');
                    }
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
                case 'c':
                    buffer[index++] = va_arg(valist, int);
                    break;
                case 'd':
                case 'i':
                    temp_int = va_arg(valist, int);
                    temp_str = itoa(temp_int, NULL);
                    if (padding) {
                        temp_str = pad(temp_str, padding, pad_with_zeros ? '0' : ' ');
                    }
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
                default:
                    if (ch == '.') {
                        pad_with_zeros = 1;
                    }
                    if (isdigit(ch)) {
                        char temp[4] = {0};
                        int ptr = 0;
                        while (isdigit(*(format + 1))) {
                            ch = *format++;
                            temp[ptr++] = ch;
                        }
                        padding = atoi(temp);
                        goto start;
                    }
                    break;
            }

            padding = 0;
            pad_with_zeros = 0;
        } else {
            buffer[index++] = ch;
        }
    }
    va_end(valist);
    buffer[index] = '\0';
    puts(buffer);
}
void puts(char* str){
    sys_req(WRITE,COM1,str,strlen(str));
}