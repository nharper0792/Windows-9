#ifndef MPX_IOSCHEDULER
#define MPX_IOSCHEDULER

#include <sys_req.h>
#include <pcb.h>

typedef enum {
	IDLE,
	IN_USE
} alloc_status;

typedef enum {
	NO_EVENT,
	EVENT
} event_status;

typedef struct ring_buffer {
	char buffer[15];
	size_t size;
	ring_buffer* head;
	ring_buffer* tail;
} ring_buffer;

typedef struct dcb {
	enum alloc_status use_status;
	enum event_status event_status;
	enum op_code cur_op;
	
} dcb;

typedef struct iocb {
	pcb* assoc_pcb;
	dcb* assoc_dcb;
	enum op_code op_type;
} iocb;

#endif