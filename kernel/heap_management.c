#include <heap_management.h>
#include <memory.h>
#include <mpx/vm.h>

mcbList mcb_freeList;
mcbList mcb_allocList;

void initialize_heap(size_t size) {
	mcb* newMemb = (mcb*)sys_alloc_mem(sizeof(mcb));
	size_t startMem = (size_t)(kmalloc(size + sizeof(mcb), 0, NULL));

	newMemb->start_address = startMem;
	newMemb->size = size;
	newMemb->nextPtr = NULL;
	newMemb->prevPtr = NULL;

	mcb_freeList.headPtr = newMemb;
	mcb_allocList.headPtr = NULL;

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
