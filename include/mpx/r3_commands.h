#include "r3_commands.h"
#include <core/serial.h>
#include <system.h>
#include <mem/heap.h>
#include "mpx_supt.h"
#include <string.h>
#include "pcb.h"
#include "sys_call.h"
#include "processes.h"

#ifndef _R3_COMMANDS_H_
#define _R3_COMMANDS_H_

#include "processes.h"




struct pcb* load_R3(char *name, void* func);

void load();
void yield();

#endif