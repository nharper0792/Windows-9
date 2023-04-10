#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdio.h>
#include <string.h>
#include <mpx/command_history.h>
#include <memory.h>
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

int serial_init(device dev)
{
    int dno = serial_devno(dev);
    if (dno == -1) {
        return -1;
    }
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

int serial_out(device dev, const char* buffer, size_t len)
{
    int dno = serial_devno(dev);
    if (dno == -1 || initialized[dno] == 0) {
        return -1;
    }
    for (size_t i = 0; i < len; i++) {
        outb(dev, buffer[i]);
    }
    return (int)len;
}



int serial_poll(device dev, char* buffer, size_t len)
{
    int ind = 0;
    int count = 0;
    // insert your code to gather keyboard input via the technique of polling.
    // You must validate each key and handle special keys such as delete, back space, and
    // arrow keys


    while (1)
    {
        if (inb(dev + LSR) & 1)
        {
            // when data is available the lsb in the line status register, (COM1+5) is set
            char c = inb(dev); // reads one byte
            //update the user buffer or handle data
            //checks if buffer is full
            if (count == (int)len)
            {
                return count;
            }
            // backspace
            if (c == 8)
            {
                if (ind != 0)
                {
                    outb(dev, '\b');
                    outb(dev, ' ');
                    outb(dev, '\b');

                    buffer[ind] = '\0';

                    ind--;
                    count--;
                }
            }
            //delete
            else if (c == 127)
            {
                if (ind != 0)
                {
                    outb(dev, '\b');
                    outb(dev, ' ');
                    outb(dev, '\b');

                    buffer[ind] = '\0';

                    ind--;
                    count--;
                }
            }
            //Arrow Keys
            else if (c == '\033') {
                c = inb(dev);
                c = inb(dev);
                if(c=='A'){
                    if(count>0) {
                        addToCycled(buffer);

                        for (; ind > 0; ind--, count--) {
                            outb(dev, '\b');
                            outb(dev, ' ');
                            outb(dev, '\b');
                        }
                    }
                    char* newCommand = getFromHistory();
                    if(newCommand!=NULL) {
                        int len = strlen(newCommand);
                        strcpy(buffer,newCommand);
                        sys_free_mem(newCommand);
                        for(;ind<len;ind++,count++){
                            outb(dev,newCommand[ind]);
                        }
                    }
                }
                else if(c=='B'){
                    if(count>0) {
                        addToHistory(buffer);

                        for (; ind > 0; ind--, count--) {
                            outb(dev, '\b');
                            outb(dev, ' ');
                            outb(dev, '\b');
                        }
                    }
                    char* newCommand = getFromCycled();

                    if(newCommand!=NULL) {
                        int len = strlen(newCommand);
                        strcpy(buffer,newCommand);
                        sys_free_mem(newCommand);
                        for(;ind<len;ind++,count++){
                            outb(dev,newCommand[ind]);
                        }
                    }
                }

                else if (c == 'C') {
                    ind++;
                    serial_out(dev, "\033[C", 3);

                }
                else if (c == 'D') {
                    ind--;
                    serial_out(dev, "\033[D", 3);
                }


            }


            //enter
            else if (c == 13 || c == 10)
            {
                if (count > 0) {
                    resetHistory();
                    addToHistory(buffer);
                    buffer[ind] = '\0';
                    outb(dev, '\n');
                    outb(dev, '\r');
                    return count;
                }  
            }
            else
            {//should print out alphanumerics 
                buffer[ind] = c;
                ind++;
                count++;
                outb(dev, c);
            }
        }

    }
    return count;
}