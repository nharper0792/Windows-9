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
	(void)data;
	return 0;
}
