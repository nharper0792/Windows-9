#ifndef MPX_HEAP_H
#define MPX_HEAP_H

void initialize_heap(size_t data);

int free_memory(void* data);

void* allocate_memory(size_t data);
