//
// Created on 2/13/2023.
//

#ifndef WINDOWS_9_PCB_H
#define WINDOWS_9_PCB_H

#define MAX_NAME_LENGTH 12
enum classes{
    USER = 0,
    SYSTEM = 1
};

enum states{
    READY,RUNNING,BLOCKED,SUSPENDED,NOT_SUSPENDED
};

typedef struct pcb{
    char* name;
    enum classes class;
    int priority;
    enum states executionState;
    enum states dispatchingState;
    char stack[1024];
    char* stackPtr;
    struct pcb* nextPtr;
    struct pcb* prevPtr;
}pcb;

/**
 * @brief allocates memory for a new PCB
 * @return a pointer to the created PCB or NULL on error
 */
pcb* pcb_allocate(void);

/**
 * @brief frees the memory associated to a PCB
 * @param target: the pcb to free
 * @return int indicating success
 */
int pcb_free(pcb* target);

/**
 * @brief allocates and initializes a new PCB with the provided data
 * @param name: the name of the PCB
 * @param class: the class of the PCB, USER or SYSTEM
 * @param priority: the priotity status of the PCB
 * @return a pointer to the new PCB
 */
 pcb* pcb_setup(char* name, int class, int priority);

/**
 * @brief finds the specified name in the PCB list
 * @param name the target PCB name
 * @return a pointer to the specified PCB or NULL if not found
 */
pcb* pcb_find(const char* name);

/**
 * @brief inserts pcb into appropriate queue
 * @param newPCB the pcb to insert
 */
void pcb_insert(pcb* newPCB);

/**
 * @brief removes the target PCB from the queue, without freeing associated memory
 * @param target the PCB to dequeue
 * @return success or error code
 */
int pcb_remove(pcb* target);

#endif //WINDOWS_9_PCB_H
