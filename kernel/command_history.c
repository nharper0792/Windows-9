#include <mpx/command_history.h>
#include <linked-list.h>
#include <memory.h>
#include <string.h>
//
// Created by Owner on 3/18/2023.
//
list* history;
list* cycled;

void addToHistory(const char* command){
    if(history==NULL){
        history = createList();
    }
    int len = strlen(command);
    char* temp = (char*)sys_alloc_mem(len+1);
    strcpy(temp,command);
    temp[len]='\0';
    node* newNode = createNode((void*)temp);
    addToHead(history,newNode);
}
void addToCycled(const char* command){
    if(cycled==NULL){
        cycled = createList();
    }
    char* temp = (char*)sys_alloc_mem(strlen(command)+1);
    strcpy(temp,command);
    node* newNode = createNode((void*)temp);
    addToHead(cycled,newNode);
}

char* getFromHistory(){
    if(history==NULL || history->headPtr==NULL){
        return NULL;
    }
    node* temp = removeHead(history);
    return (char*)temp->data;
}
char* getFromCycled(){
    if(cycled==NULL || cycled->headPtr==NULL){
        return NULL;
    }
    node* temp = removeHead(cycled);
    return (char*)temp->data;
}

void resetHistory(){
    if(history==NULL){
        return;
    }
    if(cycled!=NULL) {
        while (cycled->headPtr != NULL) {
            addToHead(history, removeHead(cycled));
        }
    }
}