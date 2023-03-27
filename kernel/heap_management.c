#include <heap_management.h>
#include <memory.h>
#include <mpx/vm.h>

mcbList mcb_List;

void initialize_heap(size_t size) {
	//make memory control block for free List
	mcb* newMemb = (mcb*)sys_alloc_mem(sizeof(mcb));
	//create starting memory location
	size_t startMem = (size_t)(kmalloc(size + sizeof(mcb), 0, NULL));
	//set parameters of free memory
	newMemb->start_address = startMem;
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
	return 0;
}

void* allocate_memory(size_t data) {
	//traversing list to find location free mcb with enough space
	mcb* currentPtr;
	for (currentPtr = mcb_List.headPtr; (currentPtr->size >= data && currentPtr->flag == FREE) || currentPtr == NULL; currentPtr = currentPtr->nextPtr);

	//checking to see if location was found
	if (currentPtr->size >= data && currentPtr->flag == FREE) {
		size_t newBlockSize = currentPtr->size - data;
		
		//changes free block to allocated block
		currentPtr->flag = ALLOCATED;
		currentPtr->size = data;

		//creates new block and adds to list after new alloc block
		mcb* newBlock = sys_alloc_mem(sizeof(mcb));
		newBlock->flag = FREE;
		newBlock->size = newBlockSize;
		//get start address for new memory block

		//allocates memory in system
		//returns pointer for new allocated memory
	}

	//fallthrough (could not find free mcb block in list that will fit)
	return NULL;
}
