#include <mpx/command_history.h>
#include <linked-list.h>
#include <memory.h>
#include <string.h>
//
// Created by Owner on 3/18/2023.
//
list* history;
list* cycled;
node* current;
void addToHistory(const char* command){
    if(history==NULL){
        history = createList();
    }
    int len = strlen(command);
    char* temp = (char*)sys_alloc_mem(len+1);
    strcpy(temp,command);
//    sys_free_mem(command);
    temp[len]='\0';
    if(current){
        sys_free_mem(current->data);
        current->data = (void*) temp;
    }
    else {
        node *newNode = createNode((void *) temp);
        addToHead(history,newNode);
    }
}
void addToCycled(const char* command){
    if(cycled==NULL){
        cycled = createList();
    }
    char* temp = (char*)sys_alloc_mem(strlen(command)+1);
    strcpy(temp,command);
    if(current){
        sys_free_mem(current->data);
        current->data = (void*) temp;
    }
    else {
        node *newNode = createNode((void *) temp);
        addToHead(cycled,newNode);
    }
}

char* getFromHistory(){
    if(history==NULL || history->headPtr==NULL){
        return NULL;
    }
    if(current){
        addToHead(cycled,current);
    }
    current = removeHead(history);
    return (char*)current->data;
}
char* getFromCycled(){
    if(cycled==NULL || cycled->headPtr==NULL){
        return NULL;
    }
    if(current){
        addToHead(history,current);
    }
    current = removeHead(cycled);
    return (char*)current->data;
}

void resetHistory(){
    if(history==NULL){
        return;
    }
    if(cycled!=NULL) {
        while (cycled->headPtr != NULL) {
            addToHead(history, removeHead(cycled));
        }
        current = NULL;
    }
}