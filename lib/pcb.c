#include <pcb.h>
#include <linked-list.h>
#include <string.h>
#include <memory.h>
pcb* pcbHead;
pcb* pcb_allocate(void){
    pcb* newPCB = (pcb*)sys_alloc_mem(sizeof(pcb));
    newPCB->name = (char*)sys_alloc_mem(MAX_NAME_LENGTH+1);
    for(int i = 0;i<1024;i++){
        newPCB->stack[i]=0;
    }
    newPCB->stackPtr = &newPCB->stack[1024-sizeof(void*)];

    return newPCB;
}

int pcb_free(pcb* target){
    sys_free_mem(target->stackPtr);
    sys_free_mem(target);
    return 1;
}


pcb* pcb_setup(char* name, int class, int priority){
    pcb* newPcb = pcb_allocate();
    if(strlen(name)<MAX_NAME_LENGTH){
        strcpy(newPcb->name,name);
    }
    newPcb->class = class;
    newPcb->priority = priority;
    newPcb->executionState = READY;
    newPcb->dispatchingState = NOT_SUSPENDED;
    return newPcb;
}