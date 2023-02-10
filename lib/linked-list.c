#include <stdio.h>
#include <linked-list.h>

/*
Function Name   : get
Function Desc   : gets and returns node at index or NULL if node DNE at index
@param listPtr (list*) : pointer to list
@param index (int) : index value
@return node* : pointer to node at index, or null if DNE
*/
node* get(list* listPtr, int index) {
    //creating node to traverse and return
    node* currentPtr = listPtr->headPtr;

    //looping through list to get to index
    for(int i = 0; i < index; i++){
        //seeing if current node is NULL
        if (currentPtr == NULL) {
            //end of list so index is out of bounds
            return NULL;
        } else {
            //continue traversing
            currentPtr = currentPtr->nextPtr;
        }
    }

    //returns node pointer at index
    return currentPtr;
}

/*
Function Name   : getHead
Function Desc   : gets the head of the list and returns node
@param listPtr (list*) : pointer to list
@return node* : node pointer at head of list
*/
node* getHead(list* listPtr) {
    return listPtr->headPtr;
}

/*
Function Name   : add
Function Desc   : adds new node to the end of the list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to node to add
@return node* : pointer to node added to list
*/
node* add(list* listPtr, node* nodePtr) {
    //sees if list is empty
    if (listPtr->headPtr == NULL) {
        listPtr->headPtr = nodePtr;
        return nodePtr;
    } else {
        //creates node pointers to traverse lsit
        node* currentPtr = listPtr->headPtr;

        while (currentPtr->nextPtr != NULL) {
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
node* addToHead(list* listPtr, node* nodePtr) {
    //seeing if list is empty
    if (listPtr->headPtr == NULL) {
        listPtr->headPtr = nodePtr;
        return listPtr->headPtr;
    } else {
        //linking nodes and linking new headPtr to list
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
node* remove(list* listPtr, node* nodePtr) {
    //seeing if list is null
    if (listPtr->headPtr == NULL) {
        return NULL;
    }

    //traversing list to find node to remove
    for(node* currentPtr = listPtr->headPtr;currentPtr!=NULL;currentPtr = currentPtr->nextPtr) {
        //seeing if data matches data to be removed
        if (currentPtr->data == nodePtr->data) {
            currentPtr->prevPtr->nextPtr = currentPtr->nextPtr;
            currentPtr->nextPtr->prevPtr = currentPtr->prevPtr;
            return currentPtr;
        }
    }

    //node to remove was not found
    return NULL;
}

/*
Function Name   : removeHead
Function Desc   : removes head of list
@param listPtr (list*) : pointer to list
@param nodePtr (node*) : pointer to node
@return node* : pointer to node removed
*/
node* removeHead(list* listPtr) {
    //seeing if list is empty
    if (listPtr->headPtr == NULL) {
        return NULL;
    } else {
        //removing node and returning removed node
        node* nodePtr = listPtr->headPtr;
        listPtr->headPtr = nodePtr->nextPtr;
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
int contains(list* listPtr, node* nodePtr) {
    //creating nodes to traverse
    struct node* currentPtr = listPtr->headPtr;

    //traversing list to find nodePtr
    while (currentPtr != NULL) {
        //seeing if current node is nodePtr
        if (currentPtr->data == nodePtr->data) {
            return 1;
        }
        currentPtr = currentPtr->nextPtr;
    }

    //node was not found
    return 0;
}