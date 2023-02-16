#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

//list structure
typedef struct list {
    struct node* headPtr;
}list;

//node structure
typedef struct node {
    struct node* prevPtr;
    struct node* nextPtr;
    void* data;
}node;

/**
 creates a List object
 @return list* : pointer to list
*/
list* createList(void);

/**
 create a Node object
 @param data : void pointer to data to set in node
 @return node* : pointer to node
*/
node* createNode(void* data);

/**
 gets node at index and returns node structure, ____ if error
 @return node* : node structure
*/
node* get(list* listPtr, int index);

/**
 gets and returns head of list
 @return node* : head pointer node
*/
node* getHead(list* listPtr);

/**
 adds to certain index
 @return node* : pointer to node added
*/
node* add(list* listPtr, node* nodePtr);

/**
 adds node as the head of list
 */
node* addToHead(list* listPtr, node* nodePtr);

/**
 removes node from list
 @return node* : pointer of node that was removes
*/
node* remove(list* listPtr, node* nodePtr);

/**
 removes head from list and assigns to next in line
 @return node* : pointer of head node that was removed
*/
node* removeHead(list* listPtr);

/**
 iterates through list to see if node is contained in the list
 @param nodePtr (*node) : pointer to node to check for
 @return int : returns 1 if node is contained in the list, returns 0 if node is not in the list
 */
int contains(list* listPtr, node* nodePtr);

void* getData(node*);

#endif