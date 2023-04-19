#include <mpx/ioscheduler.h>
#include <mpx/serial.h>
#include <mpx/io.h>
#include <mpx/device.h>
#include <memory.h>


#define eflagO 1
#define eflagC 2
#define inUSE 3
#define EOImask 0x20
#define PICmask 0x21
enum uart_registers {
    RBR = 0,	// Receive Buffer
    THR = 0,	// Transmitter Holding
    DLL = 0,	// Divisor Latch LSB
    IER = 1,	// Interrupt Enable
    DLM = 1,	// Divisor Latch MSB
    IIR = 2,	// Interrupt Identification
    FCR = 2,	// FIFO Control
    LCR = 3,	// Line Control
    MCR = 4,	// Modem Control
    LSR = 5,	// Line Status
    MSR = 6,	// Modem Status
    SCR = 7,	// Scratch
};
static int initialized[4] = { 0 };

iocb* iocbHead;

dcb* DCB;

int serial_open(device dev, int speed)
{
	DCB = (dcb*)sys_alloc_mem(sizeof(dcb));
    DCB->assoc_dev = dev;
	DCB->event_status = eflagO;
	DCB->use_status = IDLE;
    DCB->input_buffer = NULL;
    DCB->output_buffer = NULL;
	int dno = serial_devno(dev);
    if (dno == -1) {
        return -1;
    }
    outb(dev + IER, 0x00);	//disable interrupts
    outb(dev + LCR, 0x80);	//set line control register
    outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit check whether baud rate is right
    outb(dev + DLM, 0x00);	//brd most significant bit
    outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
    outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte thresholdt
	outb(dev+1,0x01);
    initialized[dno] = 1;
	int mask;
	mask = inb(PICmask);
	mask = mask&~10;
	outb(PICmask,mask);
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
    (void)inb(dev);		//read bit to reset port
	return 0;
}

int serial_close(device dev)
{
    dev = dev;
	return 0;
}

int serial_read(device dev, char* buf, size_t len)
{
    dev = dev;
    buf[len] = buf[len];
	return 0;
}

int serial_write(device dev, char* buf, size_t len)
{
    if(!DCB){
        return 401;
    }
    if(buf==NULL){
        return 402;
    }
    if(len == 0){
        return 403;
    }
    if(DCB->use_status == BUSY){
        return 404;
    }
    DCB->output_buffer = buf;
    DCB->output_len = len;
    DCB->event_status = NO_EVENT;
    outb(dev,*buf);
    outb(dev + IER, inb(dev+IER)|0x02);
}

void serial_interrupt(void)
{
}

void serial_input_interrupt(dcb* dcb)
{
}

void serial_output_interrupt(dcb dcb)
{
}

void schedule_io(pcb* process,op_code op){
    iocb* currPtr = iocbHead;
    for(;currPtr != NULL; currPtr = currPtr->nextPtr){
    }
    iocb* newIocb = (iocb*)sys_alloc_mem(sizeof(iocb));
    newIocb->prevPtr = currPtr;
    newIocb->nextPtr = NULL;
    newIocb->assoc_pcb = process;
    newIocb->op_type = op;
    currPtr->nextPtr = newIocb;
}

alloc_status check_device_status(device dev){
    for(iocb* currPtr = iocbHead;currPtr!=NULL;currPtr = currPtr->nextPtr){
        if(currPtr->assoc_dcb->assoc_dev == dev){
            return BUSY;
        }
    }
    return NOT_BUSY;
}
