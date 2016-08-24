#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>	
#include <termios.h>
#include <stdint.h>
void uart0_send(uint8_t *buf, uint8_t len)
{
	int uart0_filestream = -1;

	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	unsigned char tx_buffer[264];
	unsigned char *p_tx_buffer;
	int i;
	
	p_tx_buffer = &tx_buffer[0];

	for(i=0; i<len; ++i){
		*p_tx_buffer= (char)buf[i];
		++p_tx_buffer;
	}
	
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
		if (count < 0)
		{
			printf("UART TX error\n");
		}
	}
	close(uart0_filestream);

}
