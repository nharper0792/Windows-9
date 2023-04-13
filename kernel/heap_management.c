#include <heap_management.h>
#include <memory.h>
#include <mpx/vm.h>
#include <string.h>

mcb* mcbHead;
mcb* mcbTail;


void initialize_heap(size_t size) {
	//make memory control block for free List
	//create starting memory location
	size_t startMem = (size_t)(kmalloc(size, 0, NULL));
    mcb* newMemb = (mcb*)startMem;
	//set parameters of free memory
	newMemb->start_address = (size_t)(newMemb+1);
	newMemb->size = size;
	newMemb->nextPtr = NULL;
	newMemb->prevPtr = NULL;
	newMemb->flag = FREE;
	//initialize lists
	mcbHead = newMemb;
    mcbTail = newMemb;

	return;
}

int free_memory(void* data) {
	//capture start address of memory to free
	//size_t mcbStartAddr = (size_t)data;
	size_t newSize = 0;
	//create mcb to traverse list
	mcb* currPtr = mcbHead;
	//edge case
	//if the current pointer (at the head) does not have a pointer in front or behind it, free and return
	if (currPtr->nextPtr == NULL
		&& currPtr->prevPtr == NULL) {
		currPtr->flag = FREE;
		return 0;
	}
	//setup freeing memory, set currPtr to the mcb with matching start address
	while (currPtr != NULL) {
		//if start address of current pointer is equal to data and also an allocated block
		if ((currPtr->start_address == (size_t)data)
			&& currPtr->flag == ALLOCATED) {
			break;
		}
		else {
			//update pointer to next
			currPtr = currPtr->nextPtr;
		}
	}
	//fail case, currPtr should have non-NULL value
	if (currPtr == NULL){
        return 1;
    }


	//add size to newsize total
	newSize += currPtr->size;

	//if next mcb is not null and also a free memory block
	while (currPtr->nextPtr != NULL
		&& currPtr->nextPtr->flag == FREE) {
		//add size to total
		newSize += (currPtr->nextPtr->size) + sizeof(mcb);
		//update next pointer
		currPtr->nextPtr = currPtr->nextPtr->nextPtr;
        if(currPtr->nextPtr!=NULL){
            currPtr->nextPtr->prevPtr=currPtr;
        }
	}
	//if prev mcb is not null and also a free memory block
	while (currPtr->prevPtr != NULL
		&& currPtr->prevPtr->flag == FREE) {
		//add size to total
		newSize += currPtr->prevPtr->size+sizeof(mcb);
		//update previous pointer
		currPtr->prevPtr->nextPtr = currPtr->nextPtr;
        if(currPtr->nextPtr!=NULL){
            currPtr->nextPtr->prevPtr = currPtr->prevPtr;
        }
		//set current pointer to new previous pointer
		currPtr = currPtr->prevPtr;
//        if(currPtr->prevPtr!=NULL)
//            currPtr->prevPtr->nextPtr = currPtr;
	}
	//update parameters of new (possibly bigger) mcb

    currPtr->size = newSize;
    currPtr->start_address = (size_t)(currPtr+1);
	currPtr->flag = FREE;
    data = NULL;
	//success return by default or if this point is reached (should only be in case of success)
	return 0;
}


void* allocate_memory(size_t data) {
	//traversing list to find location free mcb with enough space
	mcb* currentPtr;
	for (currentPtr = mcbHead; currentPtr!= NULL && !(currentPtr->size >= data && currentPtr->flag == FREE) ; currentPtr = currentPtr->nextPtr){
    }
    if(currentPtr == NULL){
        return NULL;
    }
	//checking to see if location was found
	else if (currentPtr->size >= data && currentPtr->flag == FREE) {
		size_t newBlockSize = currentPtr->size - data-sizeof(mcb);
		
		//changes free block to allocated block
		currentPtr->flag = ALLOCATED;
		currentPtr->size = data;

		//creates new block and adds to list after new alloc block
		mcb* newBlock = (mcb*)(currentPtr->start_address+data);
		newBlock->flag = FREE;
		newBlock->size = newBlockSize;
		//get start address for new memory block
        newBlock->start_address = (size_t)(newBlock+1);

        newBlock->nextPtr = currentPtr->nextPtr;
        newBlock->prevPtr = currentPtr;
        currentPtr->nextPtr = newBlock;
        if(newBlock->nextPtr != NULL){
            newBlock->nextPtr->prevPtr = newBlock;
        }

		//allocates memory in system
		//returns pointer for new allocated memory

        return (void*)currentPtr->start_address;
	}
	//fallthrough (could not find free mcb block in list that will fit)
	return NULL;
}

mcb* getHeadMcb(void) {
	return mcbHead;
}

int power(int x, int y) {
	if (y == 0) {
        return 1;
    }

    int answer = 1;
    for (int i = 0; i < y; i++) {
        answer *= x;
    }
    return answer;
}

int hexToInt(char* hexNumber) {
	int integer = 0;
    int digit = 0;
    int count = 0;

    for (int i = strlen(hexNumber); i >= 0; i--) {
        if (hexNumber[i] != '\0') {
            if (hexNumber[i] == 'a' || hexNumber[i] == 'A') {
                digit = 10;
            } else if (hexNumber[i] == 'b' || hexNumber[i] == 'B') {
                digit = 11;
            } else if (hexNumber[i] == 'c' || hexNumber[i] == 'C') {
                digit = 12;
            } else if (hexNumber[i] == 'd' || hexNumber[i] == 'D') {
                digit = 13;
            } else if (hexNumber[i] == 'e' || hexNumber[i] == 'E') {
                digit = 14;
            } else if (hexNumber[i] == 'f' || hexNumber[i] == 'F') {
                digit = 15;
            } else if ((hexNumber[i]-48) >= 0 && (hexNumber[i]-48) <=9) {
                digit = hexNumber[i] - 48;
            } else {
				integer = 0;
				break;
			}

            integer = integer + (digit)*power(16, count);
            count++;
        }
    }

    return integer;
}
