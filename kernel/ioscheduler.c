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
extern void serial_isr(void*);
static int serial_devno(device dev)
{
    switch (dev) {
        case COM1: return 0;
        case COM2: return 1;
        case COM3: return 2;
        case COM4: return 3;
    }
    return -1;
}
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
//static int initialized[4] = { 0 };

//iocb* iocbHead;

dcb* DCB_array[4] = {0};

device devices[4] = {
        COM1,
        COM2,
        COM3,
        COM4
};
int serial_open(device dev, int speed)
{
    (void)speed;
	dcb* DCB = (dcb*)sys_alloc_mem(sizeof(dcb));
    DCB->assoc_dev = dev;
	DCB->event_status = eflagO;
	DCB->use_status = NOT_BUSY;
    DCB->buffer = (ring_buffer*)sys_alloc_mem(sizeof(ring_buffer));
    DCB->buffer->head = DCB->buffer->tail = 0;
	int dno = serial_devno(dev);
    if (dno == -1) {
        return -1;
    }
    idt_install(0x24,serial_isr);
    int divisor = 115200 / (long)speed; //find baud rate
    int remainder = 115200 % (long)speed;
    outb(dev + IER, 0x00);	//disable interrupts
    outb(dev + LCR, 0x80);	//set line control register
    outb(dev + DLL, (int)divisor);	//set bsd least sig bit check whether baud rate is right
    outb(dev + DLM, remainder);	//brd most significant bit
    outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
    outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte thresholdt
	outb(dev+IER,0x01);
    DCB_array[dno] = DCB;
//    initialized[dno] = 1;
    cli();
	int mask;
	mask = inb(PICmask);
    mask &= ~0x10;
	outb(PICmask,mask);
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
    sti();
    (void)inb(dev);		//read bit to reset port

	return 0;
}

int serial_close(device dev)
{
    //check if device exists
    int dno = serial_devno(dev);
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

int serial_read(device dev, char* buf, size_t len){
    //finding dcb to read from dcb list
    dcb* DCB = DCB_array[serial_devno(dev)];
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
    DCB->event_status = NO_EVENT;
    DCB->char_buffer = buf;
    DCB->buffer_len = len;
    //dev found and begins reading from ring buffer
    size_t currentSize = 0;
    ring_buffer* rb = DCB->buffer;
    cli();
    while (currentSize < len && rb->head != rb->tail) {
        if(rb->arr[rb->head]=='\n' || rb->arr[rb->head]=='\r'){
            rb->head++;
            rb->head %= sizeof(rb->arr);
            break;
        }
        buf[currentSize] = rb->arr[rb->head++];
        rb->head %= sizeof(rb->arr);
        currentSize++;
    }
    sti();
    if(currentSize==len){
        return 1;
    }
    else{
        DCB->cur_op = READ;
        return 1;
    }
    //successfully read

}

int serial_write(device dev, char* buf, size_t len)
{
    dcb* DCB = DCB_array[serial_devno(dev)];
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
    DCB->char_buffer = buf;
    DCB->buffer_len = len;
    DCB->cur_op = WRITE;
    char c = *(DCB->char_buffer);
    DCB->char_buffer++;
    DCB->buffer_progress++;
    outb(dev + THR, c);
    cli();
    outb(dev + IER, inb(dev+IER)|0x02);
    sti();
    return 1;
}

void serial_interrupt(void)
{
    dcb* DCB = DCB_array[serial_devno(COM1)];
    unsigned char interType = inb(COM1+2);
    if ((interType & 4) == 0) {
        if ((interType & 2) == 2) {
            serial_output_interrupt(DCB);
        }
        else{
            inb(DCB->assoc_dev + MSR);
        }

    }
    else if ((interType & 2) == 0) {
        serial_input_interrupt(DCB);
    }
    else{
        inb(DCB->assoc_dev + LSR);
    }
    outb(0x20, 0x20);

}

void serial_input_interrupt(dcb* dcb1)
{
    char character = inb(COM1);

    if (dcb1->cur_op != READ) {
        ring_buffer* rb = dcb1->buffer;

        if (rb->tail%sizeof(rb->arr) != rb->head-1) {
            rb->arr[rb->tail++] = character;
        }
        return;
    }
    else{
        if(dcb1->buffer_progress < dcb1-> buffer_len && (character!='\n' && character!='\r')){
            dcb1->char_buffer[dcb1->buffer_progress++] = character;
        }
        else{
            dcb1->use_status = NOT_BUSY;
            dcb1->event_status = EVENT;
            ((context*)(dcb1->iocb_head->assoc_pcb->stackPtr))->EAX = dcb1->buffer_progress;
        }
    }
}

void serial_output_interrupt(dcb* dcb1)
{
    if(dcb1->cur_op != WRITE){
        return;
    }
    if(dcb1->buffer_progress < dcb1->buffer_len){
        char c = *(dcb1->char_buffer);
        dcb1->char_buffer++;
        dcb1->buffer_progress++;
        outb(dcb1->assoc_dev + THR, c);
        return;
    }else{
        dcb1->use_status = NOT_BUSY;
        dcb1->event_status = EVENT;
        ((context*)(dcb1->iocb_head->assoc_pcb->stackPtr))->EAX = dcb1->iocb_head->buffer_len;
        outb(dcb1->assoc_dev + IER, inb(dcb1->assoc_dev+IER)&~0x02);
        return;
    }
}

void schedule_io(pcb* process, op_code op, device dev, char* buffer, size_t size){
    dcb* DCB = DCB_array[serial_devno(dev)];
    iocb* newIocb = (iocb*)sys_alloc_mem(sizeof(iocb));
    newIocb->nextPtr = NULL;
    newIocb->assoc_pcb = process;
    newIocb->op_type = op;
    newIocb->buffer_index = 0;
    newIocb->buffer = buffer;
    newIocb->buffer_len = size;
    iocb* currPtr = DCB->iocb_head;
    if(currPtr == NULL){
        DCB->iocb_head = newIocb;

        op==READ?   serial_read(dev,buffer,size) : serial_write(dev,buffer,size);
    }
    else{
        for(;currPtr!=NULL && currPtr->nextPtr != NULL; currPtr = currPtr->nextPtr);
        currPtr->nextPtr = newIocb;
    }


}

void io_complete(){
    for(size_t i = 0;i<sizeof(DCB_array);i++){
        dcb* DCB = DCB_array[i];
        if(DCB){
            if(DCB->event_status == EVENT){
                iocb* iocbHead = DCB->iocb_head;
                pcb* process = iocbHead->assoc_pcb;
                pcb_remove(process);
                process->executionState = READY;
                pcb_insert(process);
                DCB->iocb_head = iocbHead->nextPtr;
                sys_free_mem(iocbHead);
                iocbHead = DCB->iocb_head;

                DCB->event_status = NO_EVENT;
                if(iocbHead){
                    DCB->cur_op = iocbHead->op_type;
                    DCB->cur_op==READ? serial_read(devices[i],iocbHead->buffer,iocbHead->buffer_len):serial_write(devices[i],iocbHead->buffer,iocbHead->buffer_len);
                }
                else{
                    DCB->char_buffer = NULL;
                    DCB->buffer_len = 0;
                    DCB->cur_op = 0;
                    DCB->buffer_progress = 0;
                }
            }
            else{
                return;
            }
        }

    }
}

alloc_status check_device_status(device dev){
    int dno = serial_devno(dev);
    dcb* DCB = DCB_array[dno];
    if(DCB){
        if(DCB->iocb_head){
            return BUSY;
        }
        else{
            return NOT_BUSY;
        }
    }else{
        return -1;
    }
//    for(iocb* currPtr = iocbHead;currPtr!=NULL;currPtr = currPtr->nextPtr){
//        if(currPtr->assoc_dcb->assoc_dev == dev){
//            return BUSY;
//        }
//    }
//    return NOT_BUSY;
}
