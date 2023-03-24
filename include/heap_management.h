#ifndef MPX_HEAP_H
#define MPX_HEAP_H

#include <linked-list.h>

enum mem_flag {
	FREE = 0,
	ALLOCATED = 1
};

typedef struct memb {
	size_t start_address;
	size_t size;
	struct memb* nextPtr;
	struct memb* prevPtr;
}memb;

extern list* memb_freeList;
extern list* memb_allocList;

void initialize_heap(size_t data);

int free_memory(void* data);

void* allocate_memory(size_t data);
