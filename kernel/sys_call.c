#include <mpx/sys_call.h>
#include <pcb.h>
#include <sys_req.h>
#include <stdlib.h>

pcb* currentProcess = NULL;
context* idleing = NULL;

context* sys_call(context* current){
    pcb* readyHead = (pcb *) removeHead(ready)->data;
    if(idleing == NULL){
        idleing = current;
    }
    else{
        if(current->EAX == IDLE) {
            currentProcess = readyHead;
            currentProcess->executionState = READY;
            pcb_insert(currentProcess);
            current->EAX = 0;
        }else if(current->EAX == EXIT) {
            pcb_free(currentProcess);
            currentProcess = NULL;
            current->EAX = 0;
        }else {
            current->EAX = -1;
            return current;
        }
        //if null, set EAX as -1,
        //else set current operation process to ready head,
        //remove from ready queue and set EAX to 0
        if()
    }
}
