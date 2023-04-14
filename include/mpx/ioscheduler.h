#ifndef MPX_IOSCHEDULER
#define MPX_IOSCHEDULER

#include <sys_req.h>
#include <pcb.h>
#include <mpx/device.h>
#include <ctype.h>
#include <stddef.h>

typedef enum {
	INACTIVE,
	IN_USE
} alloc_status;

typedef enum {
	NO_EVENT,
	EVENT
} event_status;

typedef struct ring_buffer {
	char buffer[16];
	size_t size;
	struct ring_buffer* head;
	struct ring_buffer* tail;
} ring_buffer;

typedef struct dcb {
	enum alloc_status use_status;
	enum event_status event_status;
	enum op_code cur_op;
	iocb* assoc_iocb;
	ring_buffer* buffer;
} dcb;

typedef struct iocb {
	iocb* nextPtr;
	iocb* prevPtr;
	pcb* assoc_pcb;
	dcb* assoc_dcb;
	enum op_code op_type;
} iocb;

int serial_open(device dev, int speed);

int serial_close(device dev);

int serial_read(device dev, char* buf, size_t len);

int serial_write(device dev, char* buf, size_t len);

void serial_interrupt(void);

void serial_input_interrupt(struct dcb* dcb);

void serial_output_interrupt(struct dcb dcb);

void schedule_io(pcb* process);

int check_device_status(pcb* process);

#endif