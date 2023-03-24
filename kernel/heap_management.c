#include <heap_management.h>
#include <mpx/vm.h>
size_t startMem;

void initialize_heap(size_t size) {
	startMem =kmalloc(size + sizeof(memb),0, NULL);
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
