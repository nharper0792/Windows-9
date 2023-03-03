#include <mpx/sys_call.h>
#include <pcb.h>
#include <sys_req.h>
#include <stdlib.h>

pcb* currentProcess = NULL;
context* idleing = NULL;

context* sys_call(context* current){
    if(idleing == NULL){
        idleing = current;
    }
   if(current->EAX == IDLE) {
       pcb *nextProcess = (pcb *) removeHead(ready)->data;
       if (nextProcess != NULL) {
           nextProcess->stackPtr = (char *) current;
           pcb_insert(nextProcess);
           return current;
       } else {
           current->EAX = 0;
           return current;
       }
   }else if(current->EAX == EXIT) {
       context *res = idleing;
       idleing = NULL;
       res->EAX = 0;
       return res;
   }else {
       current->EAX = -1;
       return current;
   }
}
