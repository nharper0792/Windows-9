#include <mpx/sys_call.h>
#include <pcb.h>
#include <sys_req.h>
#include <stdlib.h>

pcb* currentProcess = NULL;//running process (NOT comhand)
context* idleing = NULL;

context* sys_call(context* current){
    node* readyHead = NULL;
    if(current->EAX == IDLE|| current->EAX == EXIT){
        readyHead = removeHead(ready);
    if(idleing == NULL){
        idleing = current;
    }

        if(current->EAX == IDLE) {
            if(readyHead == NULL){
                current->EAX = NULL;
                return current;
            }
            if(currentProcess!=NULL){
                //something was running

                //set it's state to ready and put in the ready queue
                currentProcess->executionState = READY;
                // save the current context as the stack top of currentProcess

                currentProcess->stackPtr = (char*)current;
                pcb_insert(currentProcess);
                current->EAX = 0;
            }
            currentProcess = (pcb*)readyHead->data;
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
        if(readyHead==NULL){
            current->EAX = -1;
            //return the saved context "idleing"
            return idleing;
        }
        else {
            currentProcess = (pcb*)readyHead->data;
            current->EAX = 0;
            //set current state to running
            currentProcess->dispatchingState = RUNNING;
            return (context*) currentProcess->stackPtr;
            // return stack ptr of current process
        }}else{
            current-> EAX =-1;
        }

    return current;
}
