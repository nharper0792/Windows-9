#ifndef MPX_HEAP_H
#define MPX_HEAP_H

#include <linked-list.h>
#include <stddef.h>
enum mem_flag {
	FREE = 0,
	ALLOCATED = 1
};

typedef struct mcb {
	size_t start_address;
	size_t size;
	struct mcb* nextPtr;
	struct mcb* prevPtr;
}mcb;

typedef struct mcbList {
	struct mcb* headPtr;
}mcbList;


void initialize_heap(size_t size);

int free_memory(void* data);

void* allocate_memory(size_t data);

#endif