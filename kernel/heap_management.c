#include <heap_management.h>
#include <memory.h>
#include <mpx/vm.h>

mcbList mcb_List;

void initialize_heap(size_t size) {
	//make memory control block for free List
	//create starting memory location
	size_t startMem = (size_t)(kmalloc(size, 0, NULL));
    mcb* newMemb = (mcb*)startMem;
	//set parameters of free memory
	newMemb->start_address = startMem+sizeof(mcb);
	newMemb->size = size;
	newMemb->nextPtr = NULL;
	newMemb->prevPtr = NULL;
	newMemb->flag = FREE;
	//initialize lists
	mcb_List.headPtr = newMemb;

	return;
}

int free_memory(void* data) {
	(void)data;
	return 1;
}

void* allocate_memory(size_t data) {
	//traversing list to find location free mcb with enough space
	mcb* currentPtr;
	for (currentPtr = mcb_List.headPtr; currentPtr != NULL  && (currentPtr->size < data || currentPtr->flag == ALLOCATED); currentPtr = currentPtr->nextPtr){

    }
    if(currentPtr == NULL){
        return NULL;
    }
	//checking to see if location was found
	else if (currentPtr->size >= data && currentPtr->flag == FREE) {
		size_t newBlockSize = currentPtr->size - data;
		
		//changes free block to allocated block
		currentPtr->flag = ALLOCATED;
		currentPtr->size = data;

		//creates new block and adds to list after new alloc block
		mcb* newBlock = (mcb*)(currentPtr->start_address+data);
		newBlock->flag = FREE;
		newBlock->size = newBlockSize;
		//get start address for new memory block
        newBlock->start_address = (size_t)newBlock+1;

        newBlock->nextPtr = currentPtr->nextPtr;
        newBlock->prevPtr = currentPtr;
        currentPtr->nextPtr = newBlock;
        if(newBlock->nextPtr != NULL){
            newBlock->nextPtr->prevPtr = newBlock;
        }
        return (void*)newBlock->start_address;

		//checks to see where block is within list and adds new free block to list
		//it will not need to replace headPtr so nothing is added for that
		if (currentPtr->prevPtr != NULL && currentPtr->nextPtr == NULL) {
			//end of list
			newBlock->nextPtr = NULL;
			newBlock->prevPtr = currentPtr;
			currentPtr->nextPtr = newBlock;
		} else {
			//somewhere within list
			newBlock->prevPtr = currentPtr;
			newBlock->nextPtr = currentPtr->nextPtr;
			currentPtr->nextPtr->prevPtr = newBlock;
			currentPtr->nextPtr = newBlock;
		}

		//allocates memory in system
		//returns pointer for new allocated memory
		
	}

	//fallthrough (could not find free mcb block in list that will fit)
	return NULL;
}
