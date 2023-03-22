#include "mpx/r3_commands.h"
#include <string.h>
#include <pcb.h>
#include "mpx/sys_call.h"
#include "processes.h"


void yield() {
    sys_req(IDLE);
}


struct pcb* r3_load(char* name, void* func){
    pcb* new = pcb_setup(name, USER, 0); 
    new->executionState = NOT_SUSPENDED;
    context* con = (context*)new->stackPtr;
    memset(con, 0, sizeof(context));
    con->fs = 0x10;
    con->gs = 0x10;
    con->ds = 0x10;
    con->es = 0x10;
    con->CS = 0x08;
    con->EBP = (int)new->stackPtr;
    con->ESP = (int)new->stack; // might not be esi may be esp
    con->EIP = (int)func; 
    con->EFLAGS = 0x0202;
    pcb_insert(new);
    return new;

}

void load(){
    r3_load("Proc1", &proc1);
    r3_load("Proc2", &proc2);
    r3_load("Proc3", &proc3);
    r3_load("Proc4", &proc4);
    r3_load("Proc5", &proc5);
}

