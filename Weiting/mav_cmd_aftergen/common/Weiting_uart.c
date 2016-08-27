#include <cstdio>
#include <unistd.h>            //Used for UART
#include <fcntl.h>            //Used for UART
#include <termios.h>        //Used for UART
#include <stdint.h>        // for uint_8
using namespace std;

class uart{
public:
    uart();
    ~uart();
    void uart0_open();
    void uart0_close();
    void uart0_send(uint8_t *buf, uint8_t len);
    inline int uart0_char_available();
    char uart0_get_char();
private:
    int uart_filestream;
    int rx_length;
    bool is_open;
};

uart::uart():uart_filestream(-1),rx_length(0),is_open(false){


    uart_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);        //Open in non blocking read/write mode
    if (uart_filestream == -1)
    {
        //ERROR - CAN'T OPEN SERIAL PORT
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }
    else{
        is_open = true;
    }

    struct termios options;
    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;        //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);
}

uart::~uart(){
    if (is_open){
        close(uart_filestream);
    }
}

void uart::uart0_open(){

    if (!is_open){
        uart_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);        //Open in non blocking read/write mode
        if (uart_filestream == -1)
        {
            //ERROR - CAN'T OPEN SERIAL PORT
            printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        }
        else{
            is_open = true;
        }

        struct termios options;
        tcgetattr(uart_filestream, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;        //<Set baud rate
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart_filestream, TCIFLUSH);
        tcsetattr(uart_filestream, TCSANOW, &options);
    }
}

void uart::uart0_close(){
    if (is_open){
        close(uart_filestream);
        is_open = true;
        uart_filestream = -1;
    }
}

void uart::uart0_send(uint8_t *buf, uint8_t len)
{
    if (is_open){
        unsigned char tx_buffer[264];
        unsigned char *p_tx_buffer;
        int i;

        p_tx_buffer = &tx_buffer[0];

        for (i = 0; i<len; ++i){
            *p_tx_buffer = (char)buf[i];
            ++p_tx_buffer;
        }

        if (uart_filestream!= -1)
        {
            int count = write(uart_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
            if (count < 0)
            {
                printf("UART TX error\n");
            }
        }
    }
    else{
        printf("Error - UART has not open yet!\n");
    }

}

inline int uart::uart0_char_available(){
    if (rx_length <= 0){
        return 0;
    }
    else{
        return 1;
    }
}

char uart::uart0_get_char(){
    if (is_open){
        if (uart_filestream != -1)
        {
            // Read up to 255 characters from the port if they are there
            unsigned char rx_buffer;
            int rx_length = read(uart_filestream, (void*)rx_buffer, 1);        //Filestream, buffer to store in, number of bytes to read (max)
            if (rx_length <= 0)
            {
                printf("Error - UART has on available charactor!\n");
            }
            return rx_buffer;
        }
    }
}
