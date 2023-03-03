//
// Created by Owner on 3/3/2023.
//

#ifndef WINDOWS_9_SYS_CALL_H
#define WINDOWS_9_SYS_CALL_H
typedef{
    int EAX;
    int EBX;
    int ECX;
    int EDX;
}context;



context* sys_call(context* currentContext);
#endif //WINDOWS_9_SYS_CALL_H
