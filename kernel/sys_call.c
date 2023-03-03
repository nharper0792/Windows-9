#include <mpx/sys_call.h>
#include <pcb.h>
#include <sys_req.h>

pcb* currentProcess = NULL;
context* idleing = NULL;

context* sys_call(context* current){
    if(idleing == NULL){
        idleing = current;
    }
    switch(current->EAX){
        case IDLE:
            if(ready->head!=NULL){
                currentProcess = ready->headPtr;
                ready->headPtr = ready->headPtr->nextPtr;
                currentProcess->stackPtr = (char*)current;
                pcb_insert(currentProcess);
            }
            else{
                current->EAX = 0;
                return current;
            }
        case EXIT:
            context* res = idleing;
            idleing = NULL;
            res->EAX = 0;
            return res;
        default:
            current->EAX = -1;
            return current;
    }
}
