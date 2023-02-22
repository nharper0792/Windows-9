#include <pcb.h>
#include <linked-list.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>

void addToReady(list* listPtr, node* nodePtr);
int getPriority(node* nodePtr);
list* ready;
list* blocked;
list* suspendedReady;
list* suspendedBlocked;
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
    sys_free_mem(target->name);
    return sys_free_mem(target);
}


pcb* pcb_setup(const char* name, int class, int priority){
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

pcb* pcb_find(const char* name){
    list* lists[] = {ready,blocked,suspendedReady,suspendedBlocked};
    size_t listlen = sizeof(lists)/sizeof(lists[0]);
    for(size_t i = 0;i<listlen;i++){
        while(lists[i]==NULL){i++;}
        for(node* currPtr = getHead(lists[i]);currPtr !=NULL && i<listlen;currPtr = currPtr->nextPtr){
            if(strcmp(((pcb*)getData(currPtr))->name,name) == 0){
                return (pcb*)getData(currPtr);
            }
        }
}
    return NULL;
}
void addToReady(list* listPtr, node* nodePtr){
    node* headPtr = getHead(listPtr);
    if(headPtr == NULL){
        add(listPtr,nodePtr);
    }
    else{
        node* currPtr;
        for(currPtr = headPtr;currPtr->nextPtr != NULL && getPriority(currPtr)<getPriority(nodePtr);currPtr = currPtr->nextPtr);

        if(currPtr->nextPtr != NULL){
        currPtr->nextPtr->prevPtr = nodePtr;
        }
        nodePtr->nextPtr = currPtr->nextPtr;
        currPtr->nextPtr = nodePtr;
        nodePtr->prevPtr = currPtr;
    }
}
int getPriority(node* nodePtr){
    return ((pcb*)getData(nodePtr))->priority;
}

void pcb_insert(pcb* newPcb){
    node* newNode = createNode(newPcb);
    if(newPcb->executionState == READY){
        if(newPcb->dispatchingState == SUSPENDED){
            if(suspendedReady == NULL){
                suspendedReady = createList();
            }
            add(suspendedReady,newNode);
        }
        else{
            if(ready == NULL){
                ready = createList();
            }
            addToReady(ready,newNode);
        }
    }
    else{
        if(newPcb->dispatchingState == SUSPENDED){
            if(suspendedBlocked == NULL){
                suspendedBlocked = createList();
            }
            add(suspendedBlocked,newNode);
        }
        else{
            if(blocked == NULL){
                blocked = createList();
            }
            add(blocked,newNode);
        }
    }
}

int pcb_remove(pcb* target){
    node* wrapper = createNode(target);
    if(remove(ready,wrapper)!=NULL){
        return 0;
    }
    if(remove(blocked,wrapper)!=NULL){
        return 0;
    }
    if(remove(suspendedReady,wrapper)!=NULL){
        return 0;
    }
    if(remove(suspendedBlocked,wrapper)!=NULL){
        return 0;
    }
    return 1;
}
int pcb_create(char *procName, int procClass, int procPriority)
{

  if (pcb_find(procName) != NULL)
  {
    printf("error: inputted name already exists\n");
    return 1;
  }
   if (procName == NULL || strlen(procName) >= MAX_NAME_LENGTH)
  {
    printf("error: Inputted PCB name is either NULL or longer than 15 characters\n");
    return 1;
  }
  if (procPriority < 0 || procPriority > 9)
  {
    printf("error: PCB priority must be between 0 and 9 inclusive\n");
    return 1;
  }
  if (procClass != USER && procClass != SYSTEM)
  {
    printf("error: Input must either be 1 or 2\n");
    return 1;
  }

  pcb_insert(pcb_setup(procName, procClass, procPriority));

  printf("PCB was created successfully\n");
  return 0;
}
