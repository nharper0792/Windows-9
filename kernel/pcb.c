#include <pcb.h>
#include <linked-list.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <mpx/sys_call.h>

list* ready;
list* blocked;
list* suspendedReady;
list* suspendedBlocked;

void addToReady(list* listPtr, node* nodePtr);
int getPriority(node* nodePtr);

pcb* pcb_allocate(void){
    pcb* newPCB = (pcb*)sys_alloc_mem(sizeof(pcb));
    if(newPCB == NULL){
        return NULL;
    }
    newPCB->name = (char*)sys_alloc_mem(MAX_NAME_LENGTH);
    if(newPCB->name == NULL){
        sys_free_mem(newPCB);
        return NULL;
    }
    for(int i = 0;i<1024;i++){
        newPCB->stack[i]=0;
    }
    newPCB->stackPtr = (char *)(newPCB->stack +1022) - sizeof(context);

    return newPCB;
}

int pcb_free(pcb* target){
    if(sys_free_mem(target->stackPtr)){
        return 1;
    }
    if(sys_free_mem(target->name)){
        return 1;
    }
    if(sys_free_mem(target)){
        return 1;
    }
    return 0;
}


pcb* pcb_setup(const char* name, int class, int priority){
    if(pcb_find(name) != NULL || strlen(name)>MAX_NAME_LENGTH || (class != USER && class!= SYSTEM) || priority<0 || priority>9){
        return NULL;
    }
    pcb* newPcb = pcb_allocate();
    if(newPcb == NULL){
        return NULL;
    }
    strcpy(newPcb->name,name);

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
        if(lists[i]!=NULL){
        for(node* currPtr = getHead(lists[i]);currPtr !=NULL && i<listlen;currPtr = currPtr->nextPtr){
            if(strcmp(((pcb*)getData(currPtr))->name,name) == 0){
                return (pcb*)getData(currPtr);
            }
        }
        }
}
    return NULL;
}
void addToReady(list* listPtr, node* nodePtr){
    node* headPtr = getHead(listPtr);
    if(headPtr == NULL || getPriority(headPtr)>getPriority(nodePtr)){
        addToHead(listPtr,nodePtr);
    }
    else{
        node* currPtr;
        for(currPtr = headPtr; currPtr->nextPtr != NULL && getPriority(currPtr)>=getPriority(nodePtr); currPtr = currPtr->nextPtr); //issue here

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
            addToReady(suspendedReady,newNode);
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
    node* temp;
    if(ready != NULL && (temp=remove(ready,wrapper))!=NULL){
        sys_free_mem(temp);
//        sys_free_mem(wrapper);
        return 0;
    }
    if(blocked != NULL && (temp=remove(blocked,wrapper))!=NULL){
        sys_free_mem(temp);
//        sys_free_mem(wrapper);
        return 0;
    }
    if(suspendedReady != NULL && (temp=remove(suspendedReady,wrapper))!=NULL){
        sys_free_mem(temp);
//        sys_free_mem(wrapper);
        return 0;
    }
    if(suspendedBlocked != NULL && (temp=remove(suspendedBlocked,wrapper))!=NULL){
        sys_free_mem(temp);
//        sys_free_mem(wrapper);
        return 0;
    }
    return 1;
}
int pcb_createcheck(char* procName)
{
    char nullCheck[] = "NULL";
    if (pcb_find(procName) != NULL)
        return -1;
    if (strcasecmp(procName, nullCheck) == 0)
        return -2;
    if (strlen(procName) >= MAX_NAME_LENGTH)
        return -3;

    return 1;
}

list* getList(int entry) {
    if (entry == 1)
        return ready;
    if (entry == 2)
        return blocked;
    if (entry == 3)
        return suspendedReady;
    if (entry == 4)
        return suspendedBlocked;
    return NULL;
}
