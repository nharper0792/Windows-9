#include <mpx/sys_call.h>
#include <pcb.h>
#include <sys_req.h>
#include <stdlib.h>
#include <mpx/ioscheduler.h>
#include <mpx/interrupts.h>
pcb* currentProcess = NULL;//running process (NOT comhand)
context* idleing = NULL;
// TODO handle EAX being READ or WRITE
//      The device will be in EBX, buffer in ECX, and buffer size in EDX
//      Check to see whether the requested device is currently in use
//       If not, you can directly call the appropriate driver function (i.e.
//       serial read() or serial write())
//      otherwise, schedule it with I/O scheduler

/**TODO When Scheduling I/O, move process to blocked state
        dispatch a new process as if the op were IDLE
        each transfeerred byte will trigger an interrupt
            device driver ISR will be called
            if the operation is complete, set the event flag
        Each time sys call() is called, check for any available Event Flags
            For any that are set, perform the required completion sequence
**/
context* sys_call(context* current){
    while(!ready->headPtr){
        io_complete();
        sti();
    }
    cli();
    node* readyHead = NULL;
    if(current->EAX == IDLE|| current->EAX == EXIT){
        readyHead = removeHead(ready);
    if(idleing == NULL){
        idleing = current;
    }

        if(current->EAX == IDLE) {
            if(readyHead == NULL){
                current->EAX = 0;
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
        }
    }
    else if (current->EAX == READ || current->EAX == WRITE){
        int dev = current->EBX;
        char* buffer = (char*)current->ECX;
        size_t size = (size_t)current->EDX;
        //check if dev in current->EBX is not busy
        //if so, call the driver function
        //check the state of the DCB


            schedule_io(currentProcess, current->EAX, dev, buffer, size);
        readyHead = removeHead(ready);
        if(readyHead == NULL){
            current->EAX = 0;
            return current;
        }
        if(currentProcess!=NULL){
            //something was running
            //set it's state to blocked and put in the blocked queue
            pcb_remove(currentProcess);
            currentProcess->executionState = BLOCKED;
            pcb_insert(currentProcess);
            // save the current context as the stack top of currentProcess
            currentProcess->stackPtr = (char*)current;
            current->EAX = 0;
        }
        currentProcess = (pcb*)readyHead->data;
    }
    else{
            current-> EAX =-1;
    }

    return current;
}
