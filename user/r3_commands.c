#include "r3_commands.h"
#include <string.h>
#include <io.h>
#include "pcb.h"
#include "sys_call.h"
#include "processes.h"


void yield() {
    sys_req(IDLE);
  }


struct pcb* r3_load(char* name, void* func){
    pcb* new = setup_pcb(name, USER, 0); 
    new->executionState = NOT_SUSPENDED;
    context* con = (context*)new->stackPtr;
    memset(con, 0, sizeof(context));
    con->fs = 0x10;
    con->gs = 0x10;
    con->ds = 0x10;
    con->es = 0x10;
    con->CS = 0x08;
    con->EBP = new->stack;
    con->ESI = new->stackPtr; // might not be esi may be esp
    con->EIP = func; 
    con->EFLAGS = 0x0202;
    insert_pcb(new);
    return new;

}

void load(){
    r3_load("Proc1", &proc1);
    r3_load("Proc2", &proc2);
    r3_load("Proc3", &proc3);
    r3_load("Proc4", &proc4);
    r3_load("Proc5", &proc5);
}

