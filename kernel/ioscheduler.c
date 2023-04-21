#include <mpx/ioscheduler.h>
#include <mpx/serial.h>
#include <mpx/io.h>
#include <mpx/device.h>
#include <mpx/serial.h>
#include <mpx/interrupts.h>
#include <mpx/sys_call.h>
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

//iocb* iocbHead;

dcb* DCB_array[4] = {0};

device devices[4] = {
        COM1,
        COM2,
        COM3,
        COM4
};
int get_devno(device dev);
int serial_open(device dev, int speed)
{
    (void)speed;
	dcb* DCB = (dcb*)sys_alloc_mem(sizeof(dcb));
    DCB->assoc_dev = dev;
	DCB->event_status = eflagO;
	DCB->use_status = NOT_BUSY;
	int dno = get_devno(dev);
    if (dno == -1) {
        return -1;
    }
    int baud_rate = 115200 / (long)speed; //find baud rate

    outb(dev + IER, 0x00);	//disable interrupts
    outb(dev + LCR, 0x80);	//set line control register
    outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit check whether baud rate is right
    outb(dev + DLM, 0x00);	//brd most significant bit
    outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
    outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte thresholdt
	outb(dev+1,0x01);
    DCB_array[dno] = DCB;
//    initialized[dno] = 1;
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
    //check if device exists
    int dno = get_devno(dev);
    if (dno == -1) {
        return 201;
    }
    dcb* DCB = DCB_array[dno];
    //set DCB status
    DCB->event_status = eflagC;
    //set values to 0x00
    outb(dev + MSR, 0x00); //modem status register
    outb(dev + IER, 0x00); //interrupt enable register
    //PIC mask changes
    int mask;
    //TODO : finish PIC masking, last to do
    mask = inb(PICmask);
  //  mask |= (1 < < 7);
    outb(PICmask, mask);


    //BUG TEST THE MESS OUTTA IT, check each parameter
    return 1;
}

int serial_read(device dev, char* buf, size_t len)
{
    //finding dcb to read from dcb list
    dcb* DCB = DCB_array[get_devno(dev)];
    iocb* currentPtr;
    for (currentPtr = DCB->iocb_head; currentPtr != NULL && currentPtr->assoc_dcb->assoc_dev != dev; currentPtr = currentPtr->nextPtr);
    if (currentPtr == NULL) {
        //dev could not be found
        return -1;
    } 



    //checking to see if device is open
    if (DCB == NULL) {
        return 301;
    }

    //checking to see if valid buffer address
    if (buf == NULL) {
        return 302;
    }

    //checking to see if valid count address and or count value
    if (len < 0) {
        return 303;
    }

    //checking to see if device is busy
    if (DCB->use_status == BUSY) {
        return 304;
    }
    
    //dev found and begins reading from ring buffer
    int i = 0;
    size_t currentSize = 0;
    while (currentSize < len && i < 16) {
        buf[i] = DCB->buffer->buffer[i];
        i++;
        currentSize++;
    }

    //successfully read
    return 0;
}

int serial_write(device dev, char* buf, size_t len)
{
    dcb* DCB = DCB_array[get_devno(dev)];
    if(!DCB){
        return 401;
    }
    if(buf==NULL){
        return 402;
    }
    if(len <= 0){
        return 403;
    }
    if(DCB->use_status == BUSY){
        return 404;
    }
    DCB->event_status = NO_EVENT;
    DCB->cur_op = WRITE;
    outb(dev + THR, buf[DCB->iocb_head->buffer_index++]);
    outb(dev + IER, inb(dev+IER)|0x02);

    return 1;
}

void serial_interrupt(void)
{
    cli();
    dcb* DCB = DCB_array[get_devno(COM1)];
    unsigned char interType = inb(COM1 + 2);
    if ((interType & 4) == 0) {
        if ((interType & 2) == 2) {
            serial_output_interrupt(DCB);
        }

    }
    else if ((interType & 2) == 0) {
        serial_input_interrupt(DCB);
    }
    outb(COM1 + 0x20, 0x20);
    sti();
}

void serial_input_interrupt(dcb* dcb1)
{

    inb(COM1);

    if (dcb1->cur_op == READ) {
        char* buffer = dcb1->buffer->buffer;

        int index = 0;
        char character = 0;

        while (character != '\n' || index <= 16) {
            character = buffer[index];
            if (character != 0) {
                continue;
            }

            
        }
        
    }

}

void serial_output_interrupt(dcb* dcb1)
{
    if(dcb1->cur_op != WRITE){
        return;
    }
    if(dcb1->iocb_head->buffer_index < dcb1->iocb_head->buffer_len){
        outb(dcb1->assoc_dev + THR, dcb1->iocb_head->buffer[dcb1->iocb_head->buffer_index++]);
        return;
    }else{
        dcb1->use_status = NOT_BUSY;
        dcb1->event_status = NO_EVENT;
        ((context*)(dcb1->iocb_head->assoc_pcb->stackPtr))->EAX = dcb1->iocb_head->buffer_len;
    }
}

void schedule_io(pcb* process, op_code op, device dev, char* buffer, size_t size){
    dcb* DCB = DCB_array[get_devno(dev)];
    iocb* currPtr = DCB->iocb_head;
    for(;currPtr->nextPtr != NULL; currPtr = currPtr->nextPtr){
    }
    iocb* newIocb = (iocb*)sys_alloc_mem(sizeof(iocb));
    newIocb->nextPtr = NULL;
    newIocb->assoc_pcb = process;
    newIocb->op_type = op;
    newIocb->buffer_index = 0;
    newIocb->buffer = buffer;
    newIocb->buffer_len = size;
    currPtr->nextPtr = newIocb;
}

alloc_status check_device_status(device dev){
    return DCB_array[get_devno(dev)]->use_status;
//    for(iocb* currPtr = iocbHead;currPtr!=NULL;currPtr = currPtr->nextPtr){
//        if(currPtr->assoc_dcb->assoc_dev == dev){
//            return BUSY;
//        }
//    }
//    return NOT_BUSY;
}

int get_devno(device dev){
    for(unsigned int i = 0;i<sizeof(devices);i++){
        if(devices[i] == dev){
            return i;
        }
    }
    return -1;
}