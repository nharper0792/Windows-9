#ifndef MPX_IOSCHEDULER
#define MPX_IOSCHEDULER

#include <sys_req.h>
#include <pcb.h>
#include <mpx/device.h>
#include <ctype.h>
#include <stddef.h>

typedef enum alloc_status{
	BUSY,
    NOT_BUSY
} alloc_status;

typedef enum event_status{
	NO_EVENT,
	EVENT
} event_status;

typedef struct ring_buffer {
	char arr[16];
	size_t size;
	int head;
	int tail;
} ring_buffer;

typedef struct dcb {
    device assoc_dev;
	alloc_status use_status;
	event_status event_status;
	op_code cur_op;
	struct iocb* iocb_head;
	ring_buffer* buffer;
    char* char_buffer;
    size_t buffer_len;
    size_t buffer_progress;
} dcb;

typedef struct iocb {
	struct iocb* nextPtr;
	pcb* assoc_pcb;
	dcb* assoc_dcb;
    size_t buffer_index;
    char* buffer;
    size_t buffer_len;
	op_code op_type;
} iocb;

int serial_open(device dev, int speed);

int serial_close(device dev);

int serial_read(device dev, char* buf, size_t len);

int serial_write(device dev, char* buf, size_t len);

void serial_interrupt(void);

void serial_input_interrupt(struct dcb* dcb);

void serial_output_interrupt(struct dcb* dcb);

void schedule_io(pcb* process, op_code op, device dev, char* buffer, size_t size);

void io_complete();

alloc_status check_device_status(device dev);

#endif