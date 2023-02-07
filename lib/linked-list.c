#include <stdio.h>

//list structure
struct list {
    struct node* headPtr;
}list;

//node structure
struct node {
    struct node* prevPtr;
    struct node* nextPtr;

}node;

struct node* get(struct list* listPtr, int index) {
    
}

/*
Function Name   : getHead
Function Desc   : gets the head of the list and returns node
@param listPtr (list*) : pointer to list
@return node* : node pointer at head of list
*/
struct node* getHead(struct list* listPtr) {
    return listPtr->headPtr;
}

/*
Function Name   : add
Function Desc   : adds new node to the end of the list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to node to add
@return node* : pointer to node added to list
*/
struct node* add(struct list* listPtr, struct node* nodePtr) {
    //sees if list is empty
    if (listPtr->headPtr == NULL) {
        listPtr->headPtr = nodePtr;
        return nodePtr;
    } else {
        //creates node pointers to traverse lsit
        struct node* currentPtr = listPtr->headPtr;
        struct node* prevPtr = currentPtr;

        while (currentPtr->nextPtr != NULL) {
            prevPtr = currentPtr;
            currentPtr = currentPtr->nextPtr;
        }

        //adding node to list
        currentPtr->nextPtr = nodePtr;
        nodePtr->prevPtr = currentPtr;

        return nodePtr;
    }
}

/*
Funtion Name    : addToHead
Function Desc   : adds node to the head of the list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to new node
@return node* : pointer to node added
*/
struct node* addToHead(struct list* listPtr, struct node* nodePtr) {
    if (listPtr->headPtr == NULL) {
        listPtr->headPtr = nodePtr;
        return listPtr->headPtr;
    } else {
        listPtr->headPtr->prevPtr = nodePtr;
        nodePtr->nextPtr = listPtr->headPtr;
        listPtr->headPtr = nodePtr;
        return listPtr->headPtr;
    }
}

/*
Function Name   : remove
Function Desc   : remove specific node from the list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to node to remove
@param node* : pointer of node that was removed
*/
struct node* remove(struct list* listPtr, struct node* nodePtr) {
    if (listPtr->headPtr == NULL) {
        return NULL;
    }

    //creating pointers to traverse list
    struct node* currentPtr = listPtr->headPtr;
    struct node* prevPtr = currentPtr;

    while (currentPtr != NULL) {
        if (currentPtr == nodePtr) {
            struct node* tempPtr = currentPtr->prevPtr;
            tempPtr->nextPtr = currentPtr->nextPtr;
        }

        prevPtr = currentPtr;
        currentPtr = currentPtr->nextPtr;
    }
}

/*
Function Name   : removeHead
Function Desc   : removes head of list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to node
@return node* : pointer to node removed
*/
struct node* removeHead(struct list* listPtr, struct node* nodePtr) {
    if (listPtr->headPtr == NULL) {
        return NULL;
    } else {
        nodePtr->nextPtr = listPtr->headPtr;
        listPtr->headPtr = nodePtr;
        return nodePtr;
    }
}

/*
Function Name   : contains
Function Desc   : checks to see if node is within the list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to node to be checked for
@return int : returns 1 if node is contained in the list, returns 0 if node is not in the list
*/
int contains(struct list* listPtr, struct node* nodePtr) {
    //creating nodes to traverse
    struct node* currentPtr = listPtr->headPtr;
    struct node* prevPtr = currentPtr;

    while (currentPtr != NULL) {
        if (currentPtr == nodePtr) {
            return 1;
        } 

        prevPtr = currentPtr;
        currentPtr = currentPtr->nextPtr;
    }

    return 0;
}