#include "r3_commands.h"
#include <string.h>
#include "pcb.h"
#include "sys_call.h"
#include "processes.h"


void yield() {
    sys_req(IDLE, 0, NULL, NULL);
     }

// Takes a function, gives it a name, and loads it into the queue
// • Loads the R3 test processes from procsr3.c (on eCampus)
// • Each process (one per function in procsr3.c) is loaded and queued in a
// non-suspended ready state, with a name and priority of your choosing
// • Initialize and save the context for each process at the top of the PCB stack
// • CS must be 0x08, other segments 0x10
// • EBP must be the base of the PCB stack
// • ESP must be the top of the PCB stack
// • EIP must be a pointer to the function (the name of the function, without
// parenthesis, is a pointer to that function)
// • EFLAGS must be 0x0202
struct pcb* r3_load(char* name, void* func){
    pcb* new = setup_pcb(name, USER, 0); 
    new->executionState = NOT_SUSPENDED;
    context* con = (context*)(new->stackptr);
    memset(con, 0, sizeof(context));
    con->fs = 0x10;
    con->gs = 0x10;
    con->ds = 0x10;
    con->es = 0x10;
    con->cs = 0x08;
    con->ebp = new->stack;
    con->esp = new->stackptr;
    con->eip = func; 
    con->eflags = 0x0202;
    insert_pcb(new_pcb);
    return new_pcb;

}

void load(){
  r3_load("Proc1", &proc1);
  r3_load("Proc2", &proc2);
  r3_load("Proc3", &proc3);
  r3_load("Proc4", &proc4);
  r3_load("Proc5", &proc5);
}

