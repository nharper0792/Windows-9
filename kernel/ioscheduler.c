#include <ioscheduler.h>
#include <serial.h>
#include <io.h>

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

int serial_open(device dev, int speed)
{
	int dno = serial_devno(dev);
    if (dno == -1) {
        return -1;
    }
	dcb* DCB = (dcb*)DCB;
	DCB->event_status = 0;
	DCB->use_status = IDLE;
    outb(dev + IER, 0x00);	//disable interrupts
    outb(dev + LCR, 0x80);	//set line control register
    outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
    outb(dev + DLM, 0x00);	//brd most significant bit
    outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
    outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
    outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
    (void)inb(dev);		//read bit to reset port
    initialized[dno] = 1;

	return 0;
}

int serial_close(device dev)
{
	return 0;
}

int serial_read(device dev, char* buf, size_t len)
{
	return 0;
}

int serial_write(device dev, char* buf, size_t len)
{
	return 0;
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
