#include <ioscheduler.h>

int serial_open(device dev, int speed)
{
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
