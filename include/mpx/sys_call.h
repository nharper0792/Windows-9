//
// Created by Owner on 3/3/2023.
//

#ifndef WINDOWS_9_SYS_CALL_H
#define WINDOWS_9_SYS_CALL_H
typedef struct{
    int gs;
    int fs;
    int es;
    int ds;
    int ss;
    int EDI;
    int ESI;
    int EBP;
    int EBX;
    int EDX;
    int ECX;
    int EAX;
}context;



context* sys_call(context* currentContext);
#endif //WINDOWS_9_SYS_CALL_H
