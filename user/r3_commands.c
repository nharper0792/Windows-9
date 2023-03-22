#include "mpx/r3_commands.h"
#include <string.h>
#include <pcb.h>
#include "mpx/sys_call.h"
#include "processes.h"


void yield() {
    sys_req(IDLE);
}


void load(){
    pcb* pr1 = pcb_setup("proc1", USER, 0); 
    pr1->executionState = NOT_SUSPENDED;
    context* con1 = (context*)pr1->stackPtr;
    memset(con1, 0, sizeof(context));
    con1->fs = 0x10;
    con1->gs = 0x10;
    con1->ds = 0x10;
    con1->es = 0x10;
    con1->ss = 0x10;
    con1->CS = 0x08;
    // con1->ESP = (int)pr1->stackPtr;
    con1->EBP = (int)pr1->stack; // might not be esi may be esp
    con1->EIP = (unsigned int) proc1;
    con1->EFLAGS = 0x0202;
    pcb_insert(pr1);

    pcb* pr2 = pcb_setup("proc2", USER, 0); 
    pr1->executionState = NOT_SUSPENDED;
    context* con2 = (context*)pr2->stackPtr;
    memset(con2, 0, sizeof(context));
    con2->fs = 0x10;
    con2->gs = 0x10;
    con2->ds = 0x10;
    con2->es = 0x10;
    con2->ss = 0x10;
    con2->CS = 0x08;
    con2->EBP = (int)pr2->stack; // might not be esi may be esp
    con2->EIP = (unsigned int) proc2;
    con2->EFLAGS = 0x0202;
    pcb_insert(pr2);

    pcb* pr3 = pcb_setup("proc3", USER, 0); 
    pr3->executionState = NOT_SUSPENDED;
    context* con3 = (context*)pr3->stackPtr;
    memset(con3, 0, sizeof(context));
    con3->fs = 0x10;
    con3->gs = 0x10;
    con3->ds = 0x10;
    con3->es = 0x10;
    con3->ss = 0x10;
    con3->CS = 0x08;
    // con3->ESP = (int)pr3->stackPtr;
    con3->EBP = (int)pr3->stack; // might not be esi may be esp
    con3->EIP = (unsigned int) proc3;
    con3->EFLAGS = 0x0202;
    pcb_insert(pr3);

    pcb* pr4 = pcb_setup("proc4", USER, 0); 
    pr4->executionState = NOT_SUSPENDED;
    context* con4 = (context*)pr4->stackPtr;
    memset(con4, 0, sizeof(context));
    con4->fs = 0x10;
    con4->gs = 0x10;
    con4->ds = 0x10;
    con4->es = 0x10;
    con4->ss = 0x10;
    con4->CS = 0x08;
    // con4->ESP = (int)pr4->stackPtr;
    con4->EBP = (int)pr4->stack; // might not be esi may be esp
    con4->EIP = (unsigned int) proc4;
    con4->EFLAGS = 0x0202;
    pcb_insert(pr4);

    pcb* pr5 = pcb_setup("proc5", USER, 0); 
    pr1->executionState = NOT_SUSPENDED;
    context* con5 = (context*)pr5->stackPtr;
    memset(con5, 0, sizeof(context));
    con5->fs = 0x10;
    con5->gs = 0x10;
    con5->ds = 0x10;
    con5->es = 0x10;
    con5->ss = 0x10;
    con5->CS = 0x08;
    // con5->ESP = (int)pr5->stackPtr;
    con5->EBP = (int)pr5->stack; // might not be esi may be esp
    con5->EIP = (unsigned int) proc5;
    con5->EFLAGS = 0x0202;
    pcb_insert(pr5);

}


