
#include <string.h>
#include "pcb.h"
#include "sys_call.h"
#include "processes.h"

#ifndef _R3_COMMANDS_H_
#define _R3_COMMANDS_H_






struct pcb* r3_load(char *name, void* func);
void load();
void yield();

#endif