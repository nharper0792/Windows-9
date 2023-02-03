#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

//list structure
struct list;

//node structure
struct node;

/**
 gets node at index and returns node structure, ____ if error
 @return node* : node structure
*/
node* get();

/**
 gets and returns head of list
 @return node* : head pointer node
*/
node* getHead();

/**
 adds to certain index
 @return node* : pointer to node added
*/
node* add();

/**
 adds node to the end of the list
 @return node* : node that was added
*/
node* addToEnd();

/**
 removes node from list
 @return node* : pointer of node that was removes
*/
node* remove();

/**
 removes head from list and assigns to next in line
 @return node* : pointer of head node that was removed
*/
node* removeHead();

#endif